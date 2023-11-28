#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namesapce::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
void PatchBase::draw(){
    if(this->m_axType==AxesType::Axes2D){ this->draw2d(); }
    if(this->m_axType==AxesType::Axes3D){ this->draw3d(); }
}

// -*-
void PatchBase::clear(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata.clear();
    this->m_ydata.clear();
    this->m_zdata.clear();
    this->m_faces.clear();
    this->m_vertices.clear();
}

// -*-
void PatchBase::draw2d(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);

    Vector<double> vertex(3);
    Vector<int> face(3);
    double x, y;
    for(auto i=0; i < this->m_faces.size(); ++i){
        face = this->m_faces[i];
        glBegin(GL_TRIANGLES);
            x = this->coord2D_to_xaxis(this->m_vertices[face[0]][0]);
            y = this->coord2D_to_yaxis(this->m_vertices[face[0]][1]);
            glVertex2d(x, y);
            x = this->coord2D_to_xaxis(this->m_vertices[face[1]][0]);
            y = this->coord2D_to_yaxis(this->m_vertices[face[1]][1]);
            glVertex2d(x, y);
            x = this->coord2D_to_xaxis(this->m_vertices[face[2]][0]);
            y = this->coord2D_to_yaxis(this->m_vertices[face[2]][1]);
            glVertex2d(x, y);
        glEnd();
    }

    auto nf = this->m_xdata.size();
    decltype(nf) nv;
    Vector<float> rgb;
    for(auto i=0; i < nf; ++i){
        nv = this->m_xdata[i].size();
        // - edge
        if(this->m_edgeColor != "none"){
            glLineWidth(this->m_lineWidth);
            gl2psLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3f(rgb[0], rgb[1], rgb[2]);
            glBegin(GL_LINE_LOOP);
            for(auto iv=0; iv < nv; ++iv){
                auto _x = this->coord2D_to_xaxis(this->m_xdata[i][iv]);
                auto _y = this->coord2D_to_yaxis(this->m_ydata[i][iv]);
                glVertex2d(_x, _y);
            }
            glEnd();
        }

        // - face
        if(this->m_faceColor != "none"){
            auto color = Color(this->m_faceColor);
            rgb = color.to_vector();
            glColor3f(rgb[0], rgb[1], rgb[2]);

            if(this->m_cdata.size()){
                rgb = this->m_cdata[i];
                glColor3d(rgb[0], rgb[1], rgb[2]);
            }
            glBegin(GL_POLYGON);
            for(auto iv=0; iv < nv; ++iv){
                auto _x = this->coord2D_to_xaxis(this->m_xdata[i][iv]);
                auto _y = this->coord2D_to_yaxis(this->m_ydata[i][iv]);
                glVertex2d(_x, _y);
            }
            glEnd();
        }
    }
}

// -*-
void PatchBase::draw3d(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);

    auto nf = this->m_xdata.size();
    Vector<float> rgb;
    for(auto i=0; i < nf; ++i){
        auto nv = this->m_xdata[i].size();
        // - edge
        if(this->m_edgeColor!="none"){
            glLineWidth(this->m_lineWidth);
            gl2psLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3f(rgb[0], rgb[1], rgb[2]);
            glBegin(GL_LINE_LOOP);
            for(auto iv=0; iv < nv; ++iv){
                auto _x = this->coord3D_to_xaxis(this->m_xdata[i][iv]);
                auto _y = this->coord3D_to_yaxis(this->m_ydata[i][iv]);
                auto _z = this->coord3D_to_zaxis(this->m_zdata[i][iv]);
                glVertex3d(_x, _y, _z);
            }
            glEnd();
        }

        // - face
        if(this->m_faceColor != "none"){
            auto color = Color(this->m_faceColor);
            rgb = color.to_vector();
            glColor3f(rgb[0], rgb[1], rgb[2]);
            if(this->m_cdata.size()){
                rgb = this->m_cdata[i];
                glColor3d(rgb[0], rgb[1], rgb[2]);
            }
            glBegin(GL_POLYGON);
            for(auto iv=0; iv < nv; ++iv){
                auto _x = this->coord3D_to_xaxis(this->m_xdata[i][iv]);
                auto _y = this->coord3D_to_yaxis(this->m_ydata[i][iv]);
                auto _z = this->coord3D_to_zaxis(this->m_zdata[i][iv]);
                glVertex3d(_x, _y, _z);
            }
            glEnd();
        }
    }
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-