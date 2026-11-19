#pragma once
#include "Common/GameType.h"
#include <vector>
#include <utility>
// Layout from the header used by the old matched PartitionSolver constructor
// 0x00567348: four real STLport vectors and the solution mode/leftover words.
// Names and algorithm vocabulary originate in EA's GPLv3 Zero Hour source.
typedef std::pair<ObjectID,unsigned int> PairObjectIDAndUInt;
typedef std::pair<ObjectID,ObjectID> PairObjectID;
typedef std::vector<PairObjectIDAndUInt> EntriesVec;
typedef std::vector<PairObjectIDAndUInt> SpacesVec;
typedef std::vector<PairObjectID> SolutionVec;
enum SolutionType { PREFER_FAST_SOLUTION=0, PREFER_CORRECT_SOLUTION=0x7fffffff };
class PartitionSolver {
protected:
    SolutionType m_howToSolve;
    EntriesVec m_data;
    SpacesVec m_spacesForData;
    SolutionVec m_currentSolution;
    unsigned int m_currentSolutionLeftovers;
    SolutionVec m_bestSolution;
public:
    PartitionSolver(const EntriesVec &,const SpacesVec &,SolutionType);
    ~PartitionSolver();
    void solve();
    const SolutionVec &getSolution() const;
};
