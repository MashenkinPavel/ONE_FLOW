#pragma once

class ISolver{
    public:
    virtual void precalc(){};
    virtual void preliminary(){};
    virtual void perform_step(){};
    virtual void save_results(){};
    virtual ~ISolver() = 0;
};
