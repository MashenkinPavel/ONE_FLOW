#include <stdio.h>
#include <string>
#include <gtest/gtest.h>

#include <mesh.h>
#include <foam_mesh.h>

#include <fstream>
#include <vector>


TEST(MeshTest, simple_VTK_POLYHEDRON){

    // 1. Вершины куба (8 точек)
    std::vector<double> points = {
        -1, -1, -1,
         1, -1, -1,
         1,  1, -1,
        -1,  1, -1,
        -1, -1,  1,
         1, -1,  1,
         1,  1,  1,
        -1,  1,  1
    };

    // 2. Описание граней (каждая грань – список индексов вершин)
    // Индексы с 0
    std::vector<std::vector<int>> faces = {
        {0, 3, 2, 1},  // нижняя (z=-1)
        {0, 4, 7, 3},  // левая (x=-1)
        {4, 5, 6, 7},  // верхняя (z=+1)
        {5, 1, 2, 6},  // правая (x=+1)
        {0, 1, 5, 4},  // передняя (y=-1)
        {2, 3, 7, 6}   // задняя (y=+1)
    };

    // 3. Формируем CELLS для одной ячейки
    std::vector<int> cells;
    // a) Сначала общее количество целых чисел в описании ячейки (пока неизвестно, запишем позже)
    // b) Общее число вершин во всех гранях
    int totalVerts = 0;
    for (const auto& f : faces) totalVerts += f.size();
    cells.push_back(totalVerts);
    // c) Все вершины подряд (как они идут в гранях)
    for (const auto& f : faces) {
        for (int v : f) cells.push_back(v);
    }
    // d) Количество граней
    cells.push_back(static_cast<int>(faces.size()));
    // e) Для каждой грани: количество вершин и сами вершины
    for (const auto& f : faces) {
        cells.push_back(static_cast<int>(f.size()));
        for (int v : f) cells.push_back(v);
    }

    // Теперь вставляем в начало ячейки общее количество целых чисел (включая это первое число? 
    // В документации: для каждой ячейки первое число – это количество целых чисел, описывающих ячейку, 
    // НЕ включая само это первое число. Поэтому общее количество = cells.size() (после добавления всего) - 1? 
    // Но мы ещё не добавили первое число. 
    // Сделаем так: сначала построим вектор без первого числа, потом вставим его размер.
    std::vector<int> cellData;
    // Добавляем totalVerts, все вершины, количество граней, описания граней
    cellData.push_back(totalVerts);
    for (const auto& f : faces) {
        for (int v : f) cellData.push_back(v);
    }
    cellData.push_back(static_cast<int>(faces.size()));
    for (const auto& f : faces) {
        cellData.push_back(static_cast<int>(f.size()));
        for (int v : f) cellData.push_back(v);
    }
    // Теперь первое число для CELLS должно быть размером cellData
    int sizeOfCell = static_cast<int>(cellData.size());
    // Итоговый массив для всех ячеек: сначала размер ячейки, потом cellData
    std::vector<int> allCells;
    allCells.push_back(sizeOfCell);
    allCells.insert(allCells.end(), cellData.begin(), cellData.end());

    // 4. Запись в файл polyhedron.vtk
    std::ofstream f("polyhedron.vtk");
    f << "# vtk DataFile Version 3.0\n";
    f << "Polyhedron example\n";
    f << "ASCII\n";
    f << "DATASET UNSTRUCTURED_GRID\n";

    // Точки
    f << "POINTS " << points.size() / 3 << " double\n";
    for (size_t i = 0; i < points.size(); ++i) {
        f << points[i] << " ";
        if ((i+1) % 3 == 0) f << "\n";
    }

    // Ячейки
    f << "CELLS 1 " << allCells.size() << "\n";
    for (int val : allCells) f << val << " ";
    f << "\n";

    // Типы ячеек
    f << "CELL_TYPES 1\n";
    f << "42\n";  // VTK_POLYHEDRON

    f.close();
}
