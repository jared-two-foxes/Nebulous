#ifndef CONTROLLER_SCOREKEEPER_H__
#define CONTROLLER_SCOREKEEPER_H__

namespace Nebulae {

class ScoreKeeper
{
private:
  int32 m_currentScore;

  public:
    ScoreKeeper();

    void Add( int32 increment );
    void Remove( int32 decrement );

};

}

#endif // CONTROLLER_SCOREKEEPER_H__