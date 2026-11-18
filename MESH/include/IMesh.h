#pragma once
#include <string>
#include <set>
#include <array>
#include <optional>
#include <utility>
#include <map>

class Patch;


class IMesh{
public:
    virtual bool ReadMesh(const std::string &filename)  = 0;
    virtual void WriteMesh(const std::string &filename) = 0;
    virtual ~IMesh() = default;

    // ===== Counters =====
    virtual int GetFaceCount() const = 0;
    virtual int GetCellCount() const = 0;
    virtual int GetInternalFaceCount() const = 0;
    virtual int GetBoundaryFaceCount() const = 0;
    virtual int GetPatchCount() const = 0;

    // ===== Ranges for range-based for: [start, end) =====
    virtual std::pair<int,int> CellRange() const = 0;
    virtual std::pair<int,int> InternalFaceRange() const = 0;
    virtual std::pair<int,int> BoundaryFaceRange() const = 0;
    virtual std::pair<int,int> AllFaceRange() const = 0;

    // ===== Face data =====
    // Returns {owner, neighbour} — neighbour = -1 for boundary faces
    virtual std::pair<int,int> FaceCells(int faceIdx) const = 0;
    virtual std::string FaceBC(int faceIdx) const = 0;
    virtual std::array<double,3> FaceCenter(int faceIdx) const = 0;
    virtual std::array<double,3> FaceNormal(int faceIdx) const = 0;
    virtual double FaceArea(int faceIdx) const = 0;

    // ===== Cell data =====
    virtual std::set<int> CellFaceList(int cellIdx) const = 0;
    virtual std::array<double,3> CellCenter(int cellIdx) const = 0;
    virtual double CellVolume(int cellIdx) const = 0;

    // ===== Patch data =====
    virtual std::string PatchName(int patchIdx) const = 0;
    virtual std::string PatchType(int patchIdx) const = 0;
    virtual int PatchFaceStart(int patchIdx) const = 0;
    virtual int PatchFaceCount(int patchIdx) const = 0;
};

