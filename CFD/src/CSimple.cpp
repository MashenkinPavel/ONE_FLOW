#include <CSimple.h>
#include <iostream>

CSimple::CSimple(   std::shared_ptr<IMesh> sp_mesh,
                    std::shared_ptr<CGeneralSettings> sp_gsettings,
                    std::shared_ptr<Logger> sp_log ):
                    m_sp_mesh(sp_mesh), m_sp_gsettings(sp_gsettings), m_sp_log(sp_log)
{
    m_data_registry = std::make_shared<DataRegistry>();

     {
        // Внутренние грани
        for (int f = 0; f < m_sp_mesh->GetInternalFaceCount(); ++f) {
            auto [owner, neib] = m_sp_mesh->FaceCells(f);
            auto center = m_sp_mesh->FaceCenter(f);
            // ...
        }

        // Граничные условия
        for (int f = 0; f < m_sp_mesh->GetBoundaryFaceCount(); ++f) {
            int globalIdx = m_sp_mesh->GetInternalFaceCount() + f;
            std::string bc = m_sp_mesh->FaceBC(globalIdx); // "inlet", "wall", ...
            // ...
        }

        // Патчи
        for (int i = 0; i < m_sp_mesh->GetPatchCount(); ++i) {
            std::cout << m_sp_mesh->PatchName(i) << " "
                      << m_sp_mesh->PatchType(i) << "\n";
        }

        //auto[first,count] =  m_sp_mesh->CellRange();

     }
}



