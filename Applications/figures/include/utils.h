#ifndef EXPERIMENTS_UTILS_H
#define EXPERIMENTS_UTILS_H

#include <DGtal/io/boards/Board2D.h>

template<typename TType>
void paint(DGtal::Board2D& board, const TType& ds, DGtal::Color colorFill,DGtal::Color colorBorder=DGtal::Color::Silver)
{
    std::string specificStyle = ds.className() + "/Paving";
    board << DGtal::SetMode(ds.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(colorBorder,colorFill));
    board << ds;
}

std::string imagesDir();

#endif //EXPERIMENTS_UTILS_H
