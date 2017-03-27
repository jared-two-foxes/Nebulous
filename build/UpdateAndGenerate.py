import os
import sys
import time
import xml.etree.ElementTree
import subprocess
import threading
import traceback
import shutil
from cStringIO import StringIO
from collections import namedtuple

###### externals

SvnInfo = namedtuple('SvnInfo', ['localPath', 'remoteUrl', 'revision'])

def isSvnURL(line):
  return '://' in line

def parseRevision(parts):
  if len(parts) == 1:
    assert parts[0].startswith('-r')
    return int(parts[0][2:])
  else:
    assert len(parts) == 2
    assert parts[0] == '-r'
    return int(parts[1])

# returns SvnInfo
def parseExternal(externalLine):
  externalLine = externalLine.strip()
  parts = externalLine.split()
  if len(parts) in [3, 4]:
    if isSvnURL(parts[-1]):
      assert not isSvnURL(parts[0])
      return SvnInfo(parts[0], parts[-1], parseRevision(parts[1:-1]))
    else:
      assert isSvnURL(parts[-2])
      return SvnInfo(parts[-1], parts[-2], parseRevision(parts[:-2]))
  elif len(parts) == 2:
    assert isSvnURL(parts[0]) != isSvnURL(parts[1])

    urlIndex = 1
    folderIndex = 0
    if isSvnURL(parts[0]):
      urlIndex = 0
      folderIndex = 1
    
    # look for a revision lock at the end of the url
    urlParts = parts[urlIndex].split('@')
    
    if len(urlParts) == 2:
      return SvnInfo(parts[folderIndex], urlParts[0], urlParts[1])
    else:
      return SvnInfo(parts[folderIndex], parts[urlIndex], None)

  else:
    assert False, 'Unable to parse external line: "%s" %s' % (externalLine, parts)

# returns SvnInfo
def getInfo(path):
  externalsXmlStr = subprocess.check_output(['svn', 'info', '--xml', path])
  externalsXml = xml.etree.ElementTree.fromstring(externalsXmlStr)
  assert externalsXml.tag == 'info'

  entryNode = externalsXml.find('entry')
  path = entryNode.attrib['path']
  rev  = entryNode.attrib['revision']
  url  = entryNode.find('url').text

  return SvnInfo(path, url, rev)

# returns list of SvnInfo
def getExternals(path):
  externalsXmlStr = subprocess.check_output(['svn', 'propget', '--recursive', '--xml', 'svn:externals', path])
  externalsXml = xml.etree.ElementTree.fromstring(externalsXmlStr)
  assert externalsXml.tag == 'properties'

  externals = []
  for targetNode in externalsXml.iter('target'):
    targetPath = targetNode.attrib['path']
    for propertyNode in targetNode.iter('property'):
      assert propertyNode.attrib['name'] == 'svn:externals'
      for externalLine in propertyNode.text.splitlines():
        svnInfo = parseExternal(externalLine)
        fullLocalPath = os.path.join(targetPath, svnInfo.localPath)
        svnInfo = svnInfo._replace(localPath = fullLocalPath)

        externals.append(svnInfo)

  return externals

###### tools

logLock = threading.Lock()
def logLineOut(line):
  with logLock:
    print line

def logLineErr(line):
  with logLock:
    sys.stderr.write(line + '\n')

class SvnError(Exception):
  def __init__(self, info, stderr):
    Exception.__init__(self, info + '\n' + stderr)

class LineLogger:
  def __init__(self, infile, prefix = None, logFn = logLineOut):
    if prefix == None:
      prefix = '[%s] ' % threading.current_thread().name
    self.prefix = prefix
    self.infile = infile
    self.allData = StringIO()
    self.dirtyData = ''
    self.logFn = logFn
    self.thread = threading.Thread(target = self._threadPipe)
    self.thread.start()

  def _threadPipe(self):
    while True:
      data = self.infile.read(1)
      if data == '':
        return

      self.allData.write(data)

      if self.logFn:
        self.dirtyData += data
        if '\n' in self.dirtyData:
          lines = self.dirtyData.split('\n')
          for line in lines[:-1]:
            self.logFn(self.prefix + line)
          self.dirtyData = lines[-1]

  def get(self):
    self.thread.join()
    return self.allData.getvalue()

def formatTime(seconds):
  return '%ss' % int(seconds)

def asyncCall(function, args):
  class ThreadWithResult(threading.Thread):
    def __init__(self, function, args):
      threading.Thread.__init__(self)
      self.function = function
      self.args = args
      self.res = None
      self.exception = None

    def run(self):
      try:
        self.res = function(*args)
      except Exception, e:
        logLineErr(traceback.format_exc())
        self.exception = e

    def get(self):
      self.join()
      if self.exception:
        raise self.exception
      return self.res

  thread = ThreadWithResult(function, args)
  thread.start()
  return thread

def safeDelete(path):
  delPath = path + '.2delete'
  if os.path.exists(delPath):
    safeDelete(delPath)
  assert os.path.exists(path), 'Path doesnt exist: %s' % path
  os.rename(path, delPath)
  shutil.rmtree(delPath)

##### code

# returns list of stats
def update(path, remoteUrl = None, args = None, revision = None):
  asyncResults = []

  if not sys.version_info[:2] == (2, 7):
    print "Error, You need python version 2.7, please update you are using version: "
    print (sys.version)   
    return []   
   
  if not revision:
    revision = 'HEAD'

  if args == None:
    args = []

  mode = 'update'
  if remoteUrl:
    if not os.path.exists(path):
      mode = 'checkout'
    else:
      info = getInfo(path)
      if info.remoteUrl != remoteUrl:
        mode = 'switch'

  timeInit = time.time()
  cmdline = None
  if mode == 'checkout':
    logLineOut('Checking out %s to %s revision of %s with args: %s' % (os.path.relpath(path), revision, remoteUrl, args))
    cmdline = ['svn', 'checkout', '--non-interactive', '--ignore-externals', '--revision', str(revision), remoteUrl, path] + args
  elif mode == 'switch':
    logLineOut('Switching %s to %s revision of %s with args: %s' % (os.path.relpath(path), revision, remoteUrl, args))
    cmdline = ['svn', 'switch', '--ignore-ancestry', '--non-interactive', '--ignore-externals', '--revision', str(revision), remoteUrl, path] + args
  elif mode == 'update':
    logLineOut('Updating %s to %s revision with args: %s' % (os.path.relpath(path), revision, args))    
    cmdline = ['svn', 'update', '--non-interactive', '--ignore-externals', '--revision', str(revision), path] + args
  else:
    assert False, 'Unsupported mode: %s' % mode

  popen = subprocess.Popen(cmdline, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
  outLogger = LineLogger(popen.stdout, logFn = logLineOut)
  errLogger = LineLogger(popen.stderr, logFn = logLineErr)
  popen.wait()
  if popen.returncode != 0:
    raise SvnError('%s failed' % mode, errLogger.get())

  timeAfterUpdate = time.time()
  externals = getExternals(path)
  timeAfterPropget = time.time()

  for localPath, remoteUrl, externalRevision in externals:
      asyncResult = asyncCall(update, (localPath, remoteUrl, args, externalRevision))
      asyncResults.append(asyncResult)

  results = []
  for ar in asyncResults:
    ar.join()
  for ar in asyncResults:
    results += ar.get()

  timeEnd = time.time()

  durationUpdate    = timeAfterUpdate  - timeInit
  durationPropget   = timeAfterPropget - timeAfterUpdate
  durationSubUpdate = timeEnd - timeAfterPropget
  durationTotal     = timeEnd - timeInit

  return [(path, durationTotal, durationUpdate, durationPropget, durationSubUpdate)] + results

def cleanup(path):
  asyncResults = []

  timeInit = time.time()
  logLineOut('Cleaning up %s' % os.path.relpath(path))
  cmdline = ['svn', 'cleanup', '--non-interactive', path]
    
  popen = subprocess.Popen(cmdline, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
  outLogger = LineLogger(popen.stdout, logFn = logLineOut)
  errLogger = LineLogger(popen.stderr, logFn = logLineErr)
  popen.wait()
  if popen.returncode != 0:
    raise SvnError('cleanup failed', errLogger.get())

  timeAfterUpdate = time.time()
  externals = getExternals(path)
  timeAfterPropget = time.time()

  for localPath, remoteUrl, externalRevision in externals:
      asyncResult = asyncCall(cleanup, (localPath,))
      asyncResults.append(asyncResult)

  results = []
  for ar in asyncResults:
    ar.join()
  for ar in asyncResults:
    results += ar.get()

  timeEnd = time.time()

  durationUpdate    = timeAfterUpdate  - timeInit
  durationPropget   = timeAfterPropget - timeAfterUpdate
  durationSubUpdate = timeEnd - timeAfterPropget
  durationTotal     = timeEnd - timeInit

  return [(path, durationTotal, durationUpdate, durationPropget, durationSubUpdate)] + results

def extractRevision(args):
  args = list(args)
  revision = None
  revArgs = ['-r', '--revision']
  for idx, arg in enumerate(args):
    if arg in revArgs:
      revision = int(args[idx+1])
      args.pop(idx)
      args.pop(idx)
      break
    else:
      for revArg in revArgs:
        assert not arg.startswith(revArg), 'Unsupported revision format, please use separate option and value'
  return args, revision

def printStats(stats):
  for path, durationTotal, durationUpdate, durationPropget, durationSubUpdate in stats:
    print 'Path: "%s"' % os.path.relpath(path)
    print '\t total      : %s' % formatTime(durationTotal)
    print '\t self update: %s' % formatTime(durationUpdate)
    print '\t propget    : %s' % formatTime(durationPropget)
    print '\t sub update : %s' % formatTime(durationSubUpdate)
    print

if __name__ == '__main__':
  if len(sys.argv) > 1:
    path = sys.argv[1]
  else:
    path = "."
  
  args = sys.argv[2:]

  assert os.path.exists(path), 'Unable to find path: %s' % sys.argv[1]
  args, revision = extractRevision(args)

  stats = update(path, None, args, revision)

#  Remove the below so I could see more of the output of the update, Could add this back as an option
#  print
#  print 'Done updating, statistics:'
#  printStats(stats)  

  if sys.platform.startswith('win32'):
    subprocess.call(['premake_project_win32.bat'])
    os.startfile('prj\Win32\Adventures.Sln')
  if sys.platform.startswith('darwin'):
    subprocess.call(['premake_project_ios.bat'])