#include "gtest/gtest.h"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <direct.h>
#include <errno.h>

static std::string Normalize(const std::string& p) {
  std::string r = p;
  for (auto& c : r) if (c == '/') c = '\\';
  return r;
}

static bool MkdirP(const std::string& path) {
  std::string p = Normalize(path);
  for (size_t i = 0; i < p.size(); i++) {
    if (p[i] == '\\') {
      std::string sub = p.substr(0, i);
      if (!sub.empty()) { _mkdir(sub.c_str()); }
    }
  }
  int ret = _mkdir(p.c_str());
  return ret == 0 || errno == EEXIST;
}

static bool CopyOne(const std::string& src, const std::string& dst) {
  std::ifstream is(Normalize(src), std::ios::binary);
  if (!is) { fprintf(stderr, "STAGE: FAIL open src [%s]\n", src.c_str()); return false; }
  std::ofstream os(Normalize(dst), std::ios::binary);
  if (!os) { fprintf(stderr, "STAGE: FAIL open dst [%s]\n", dst.c_str()); return false; }
  os << is.rdbuf();
  bool ok = os.good();
  if (ok) fprintf(stderr, "STAGE: OK  %s\n", src.c_str());
  else    fprintf(stderr, "STAGE: FAIL write %s\n", dst.c_str());
  return ok;
}

void StageRunfilesAndAdjustCwd() {
  const char* mf = std::getenv("RUNFILES_MANIFEST_FILE");
  if (!mf) return;
  const char* ws = std::getenv("TEST_WORKSPACE");
  if (!ws) ws = "_main";

  std::vector<std::pair<std::string,std::string>> items;
  std::ifstream f(mf);
  std::string line;
  while (std::getline(f, line)) {
    auto pos = line.find(' ');
    if (pos != std::string::npos) {
      std::string logical = line.substr(0, pos);
      std::string real = line.substr(pos + 1);
      if (logical.find("tests/Assets") != std::string::npos) {
        items.push_back({logical, real});
      }
    }
  }
  f.close();
  fprintf(stderr, "STAGE: %zu manifest entries for tests/Assets\n", items.size());

  const char* td = std::getenv("TEST_TMPDIR");
  if (!td) td = ".";
  std::string base = std::string(td) + "/runfiles_staging";
  std::string assets_dir = base + "/tests/Assets";
  MkdirP(assets_dir);

  std::string prefix = std::string(ws) + "/tests/Assets/";
  for (auto& [logical, real] : items) {
    auto p = logical.find(prefix);
    if (p == std::string::npos) continue;
    std::string rel = logical.substr(p + prefix.size());
    std::string dst = assets_dir + "/" + rel;
    auto slash = dst.find_last_of("/\\");
    if (slash != std::string::npos) MkdirP(dst.substr(0, slash));
    CopyOne(real, dst);
  }

  std::string bindir = base + "/tests/simulated_bin";
  MkdirP(bindir);
  _chdir(bindir.c_str());
  fprintf(stderr, "STAGE: CWD -> %s\n", bindir.c_str());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  StageRunfilesAndAdjustCwd();
  return RUN_ALL_TESTS();
}
