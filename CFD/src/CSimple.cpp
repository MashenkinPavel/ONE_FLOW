#include <CSimple.h>

CSimple::CSimple(   std::shared_ptr<IMesh> sp_mesh,
                    std::shared_ptr<CGeneralSettings> sp_gsettings,
                    std::shared_ptr<Logger> sp_log ):
                    m_sp_mesh(sp_mesh), m_sp_gsettings(sp_gsettings), m_sp_log(sp_log)
{
    //m_sp_mesh->
}