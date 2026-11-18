#pragma once
#include <memory>

#include <ISolver.h>
#include <IMesh.h>
#include <DataRegistry.h>

class IMesh;
class CGeneralSettings;
class Logger;

class CSimple: public ISolver{
    public:

    CSimple(std::shared_ptr<IMesh> sp_mesh,
        std::shared_ptr<CGeneralSettings> sp_gsettings,
        std::shared_ptr<Logger> sp_log );
    
    private:
    std::shared_ptr<IMesh> m_sp_mesh;
    std::shared_ptr<CGeneralSettings> m_sp_gsettings;
    std::shared_ptr<Logger> m_sp_log;
    

    std::shared_ptr<double[]> sp_pressure;
    std::shared_ptr<double[]> sp_Vx;
    std::shared_ptr<double[]> sp_Vy;
    std::shared_ptr<double[]> sp_Vz;
    std::shared_ptr<double[]> sp_Mu;


    std::shared_ptr<DataRegistry> m_data_registry;


};