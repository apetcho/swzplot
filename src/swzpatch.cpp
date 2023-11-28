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

// -*-
Patch PatchBase::bar(const Vector<double>& yvec, float width){
    Vector<double> xvec(yvec.size());
    for(auto i=0; i < yvec.size(); ++i){
        xvec[i] = static_cast<double>(i+1);
    }
    return this->bar(xvec, yvec, width);
}

// -*-
Patch PatchBase::bar(const Vector<double>& xvec, const Vector<double>& yvec, float width){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_axType = AxesType::Axes2D;
    this->m_xdata.clear();
    this->m_ydata.clear();
    this->m_zdata.clear();
    double w = width*(max(xvec) - min(xvec))/xvec.size();

    Vector<double> xdata(4);
    Vector<double> ydata(4);
    for(auto i=0; i < xvec.size(); ++i){
        xdata[0] = xvec[i] - w/2.0;
        ydata[0] = 0.0;
        xdata[1] = xvec[i] + w/2.0;
        ydata[1] = 0.0;
        xdata[2] = xvec[i] + w/2.0;
        ydata[2] = yvec[i];
        xdata[3] = xvec[i] - w/2.0;
        ydata[3] = yvec[i];
        this->m_xdata.push_back(xdata);
        this->m_ydata.push_back(ydata);
    }

    return this->share();
}

// -*-
Patch PatchBase::patch(const Matrix<double>& xmat, const Matrix<double>& ymat){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_axType = AxesType::Axes2D;
    this->m_xdata = xmat;
    this->m_ydata = ymat;
    this->m_zdata.clear();
    this->m_cdata.clear();

    return this->share();
}

// -*-
Patch PatchBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Vector<double>& cvec
){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_axType = AxesType::Axes2D;
    this->m_xdata = xmat;
    this->m_ydata = ymat;
    this->m_zdata.clear();
    this->m_cdata = this->index_to_truecolor(cvec);

    return this->share();
}

// -*-
Patch PatchBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<float>& cdata
){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_axType = AxesType::Axes2D;
    this->m_xdata = xmat;
    this->m_ydata = ymat;
    this->m_zdata.clear();
    this->m_cdata = cdata;

    return this->share();
}

// -*-
Patch PatchBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat
){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_ca->m_axType = AxesType::Axes3D;
    this->m_axType = AxesType::Axes3D;
    this->m_xdata = xmat;
    this->m_ydata = ymat;
    this->m_zdata = zmat;
    this->m_cdata.clear();

    return this->share();
}

// -*-
Patch PatchBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Vector<double>& cvec
){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_ca->m_axType = AxesType::Axes3D;
    this->m_axType = AxesType::Axes3D;
    this->m_xdata = xmat;
    this->m_ydata = ymat;
    this->m_zdata = zmat;
    this->m_cdata = this->index_to_truecolor(cvec);

    return this->share();
}

// -*-
Patch PatchBase::patch(
    const Matrix<double>& xmat, const Matrix<double>& ymat,
    const Matrix<double>& zmat, const Matrix<float>& cdata
){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_ca->m_axType = AxesType::Axes3D;
    this->m_axType = AxesType::Axes3D;
    this->m_xdata = xmat;
    this->m_ydata = ymat;
    this->m_zdata = zmat;
    this->m_cdata = cdata;

    return this->share();
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-