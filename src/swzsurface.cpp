#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namespace::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
void SurfaceBase::draw(){
    if(this->m_axType==AxesType::ContourPlot){ this->draw_contour(); }
    else if(this->m_axType==AxesType::Axes3D){ this->draw3d(); }
    else{ this->draw2d(); }
}

// -*-
void SurfaceBase::clear(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    this->m_xdata.clear();
    this->m_ydata.clear();
    this->m_zdata.clear();
    this->m_cdataIndex.clear();
    this->m_cdata.clear();
    this->m_vdata.clear();
}

// -*-
void SurfaceBase::draw2d(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    auto nxi = this->m_xdata.size();
    decltype(nxi) nxj = 0;
    if(nxi){
        nxj = this->m_xdata[0].size();
    }
    auto nyi = this->m_ydata.size();
    decltype(nyi) nyj = 0;
    if(nyi){
        nyj = this->m_ydata[0].size();
    }
    auto nzi = this->m_zdata.size();
    decltype(nzi) nzj = 0;
    if(nzi){
        nzj = this->m_zdata[0].size();
    }
    std::vector<float> rgb;

    if(nxi==0){
        std::cout << __func__ << " " << __FILE__ << ":" << __LINE__ << std::endl;
        // edge
        if(this->m_edgeColor != "none"){
            glLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3d(rgb[0], rgb[1], rgb[2]);
            for( auto i=0; i < nzi; ++i){
                glBegin(GL_LINE_STRIP);
                for(auto j=0; j < nzj; ++j){
                    auto x = (
                        this->m_ca->m_xdatalim.minval +
                        (this->m_ca->m_xdatalim.maxval - this->m_ca->m_xdatalim.minval)*((1.0*j)/nzj-1)
                    );
                    x = this->coord2D_to_xaxis(x);
                    auto y = (
                        this->m_ca->m_ydatalim.minval +
                        (this->m_ca->m_ydatalim.maxval - this->m_ca->m_ydatalim.maxval)*((1.0*i)/(nzi-1))
                    );
                    y = this->coord2D_to_yaxis(y);
                    glVertex2d(x, y);
                }
                glEnd();
            }
        }
        // face
        if(this->m_faceColor != "none"){
            for( auto i=0; i < nzi-1; ++i){
                for(auto j=0; j < nzj -1; ++j){
                    auto color = Color(this->m_faceColor);
                    rgb = color.to_vector();
                    if(this->m_faceColor=="flat"){
                        rgb = this->m_cdata[i][j];
                    }
                    double x, y;
                    glColor3f(rgb[0], rgb[1], rgb[2]);
                    glBegin(GL_QUADS);
                        x = (
                            this->m_ca->m_xdatalim.minval + 
                            (this->m_ca->m_xdatalim.maxval - this->m_ca->m_xdatalim.minval)*((1.0*j)/(nzj-1))
                        );
                        x = this->coord2D_to_xaxis(x);
                        y = (
                            this->m_ca->m_ydatalim.minval +
                            (this->m_ca->m_ydatalim.maxval - this->m_ca->m_ydatalim.minval)*((1.0*i)/(nzi-1))
                        );
                        y = this->coord2D_to_yaxis(y);
                        glVertex2d(x, y);

                        x = (
                            this->m_ca->m_xdatalim.minval + 
                            (this->m_ca->m_xdatalim.maxval - this->m_ca->m_xdatalim.minval)*((1.0*(j+1))/(nzj-1))
                        );
                        x = this->coord2D_to_xaxis(x);
                        y = (
                            this->m_ca->m_ydatalim.minval +
                            (this->m_ca->m_ydatalim.maxval - this->m_ca->m_ydatalim.minval)*((1.0*i)/(nzi-1))
                        );
                        y = this->coord2D_to_yaxis(y);
                        glVertex2d(x, y);
                        x = (
                            this->m_ca->m_xdatalim.minval + 
                            (this->m_ca->m_xdatalim.maxval - this->m_ca->m_xdatalim.minval)*((1.0*(j+1))/(nzj-1))
                        );
                        x = this->coord2D_to_xaxis(x);
                        y = (
                            this->m_ca->m_ydatalim.minval +
                            (this->m_ca->m_ydatalim.maxval - this->m_ca->m_ydatalim.minval)*((1.0*(i+1))/(nzi-1))
                        );
                        y = this->coord2D_to_yaxis(y);
                        glVertex2d(x, y);

                        x = (
                            this->m_ca->m_xdatalim.minval + 
                            (this->m_ca->m_xdatalim.maxval - this->m_ca->m_xdatalim.minval)*((1.0*j)/(nzj-1))
                        );
                        x = this->coord2D_to_xaxis(x);
                        y = (
                            this->m_ca->m_ydatalim.minval +
                            (this->m_ca->m_ydatalim.maxval - this->m_ca->m_ydatalim.minval)*((1.0*(i+1))/(nzi-1))
                        );
                        y = this->coord2D_to_yaxis(y);
                        glVertex2d(x, y);
                    glEnd();
                }
            }
        } // face
    }else if(nxi==1){
        if(this->m_edgeColor!="none"){
            glLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3d(rgb[0], rgb[1], rgb[2]);
            double x, y;
            for(auto i=0; i < nyj; ++i){
                glBegin(GL_LINE_STRIP);
                    x = this->coord2D_to_xaxis(this->m_xdata[0][0]);
                    y = this->coord2D_to_yaxis(this->m_ydata[0][i]);
                    glVertex2d(x, y);
                    x = this->coord2D_to_xaxis(this->m_xdata[0][nxj-1]);
                    y = this->coord2D_to_yaxis(this->m_ydata[0][i]);
                    glVertex2d(x, y);
                glEnd();
            }

            for(auto j=0; j < nxj; ++j){
                glBegin(GL_LINE_STRIP);
                    x = this->coord2D_to_xaxis(this->m_xdata[0][j]);
                    y = this->coord2D_to_yaxis(this->m_ydata[0][0]);
                    glVertex2d(x, y);
                    x = this->coord2D_to_xaxis(this->m_xdata[0][j]);
                    y = this->coord2D_to_yaxis(this->m_ydata[0][nyj-1]);
                    glVertex2d(x, y);
                glEnd();
            }
        }

        // - face
        if(this->m_faceColor != "none"){
            double x, y;
            for(auto i=0; i < nyj-1; ++i){
                for(auto j=0; j < nxj-1; ++j){
                    // color
                    auto color = Color(this->m_faceColor);
                    rgb = color.to_vector();
                    if(this->m_faceColor=="flat"){
                        rgb = this->m_cdata[i][j];
                    }
                    glColor3f(rgb[0], rgb[1], rgb[2]);

                    glBegin(GL_QUADS);
                        x = this->coord2D_to_xaxis(this->m_xdata[0][j]);
                        y = this->coord2D_to_yaxis(this->m_ydata[0][i]);
                        glVertex2d(x, y);
                        x = this->coord2D_to_xaxis(this->m_xdata[0][j]);
                        y = this->coord2D_to_yaxis(this->m_ydata[0][i+1]);
                        glVertex2d(x, y);
                        x = this->coord2D_to_xaxis(this->m_xdata[0][j+1]);
                        y = this->coord2D_to_yaxis(this->m_ydata[0][i+1]);
                        glVertex2d(x, y);
                        x = this->coord2D_to_xaxis(this->m_xdata[0][j+1]);
                        y = this->coord2D_to_yaxis(this->m_ydata[0][i]);
                        glVertex2d(x, y);
                    glEnd();
                }
            }
        }
    }// nxi==1
    else if(nxi>1){
        if(this->m_edgeColor!="none"){
            glLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3d(rgb[0], rgb[1], rgb[2]);
            double x, y;
            for(auto i=0; i < nxi; ++i){
                //! @todo: add more style
                glBegin(GL_LINE_STRIP);
                for(auto j=0; j < nxj; ++j){
                    x = this->coord2D_to_xaxis(this->m_xdata[i][j]);
                    y = this->coord2D_to_yaxis(this->m_ydata[i][j]);
                    glVertex2d(x, y);
                }
                glEnd();
            }
            for(auto j=0; j < nxi; ++j){
                glBegin(GL_LINE_STRIP);
                for(auto i=0; i < nxj; ++i){
                    x = this->coord2D_to_xaxis(this->m_xdata[i][j]);
                    y = this->coord2D_to_yaxis(this->m_ydata[i][j]);
                    glVertex2d(x, y);
                }
                glEnd();
            }
        }
        // - face
        if(this->m_faceColor != "none"){
            double x, y;
            for(auto i=0; i < nxi-1; ++i){
                for(auto j=0; j < nxj-1; ++j){
                    auto color = Color(this->m_faceColor);
                    rgb = color.to_vector();
                    if(this->m_faceColor=="flat"){
                        rgb = this->m_cdata[i][j];
                    }
                    glColor3f(rgb[0], rgb[1], rgb[2]);
                    glBegin(GL_QUADS);
                        x = this->coord2D_to_xaxis(this->m_xdata[i][j]);
                        y = this->coord2D_to_yaxis(this->m_ydata[i][j]);
                        glVertex2d(x, y);
                        x = this->coord2D_to_xaxis(this->m_xdata[i][j+1]);
                        y = this->coord2D_to_yaxis(this->m_ydata[i][j+1]);
                        glVertex2d(x, y);
                        x = this->coord2D_to_xaxis(this->m_xdata[i+1][j+1]);
                        y = this->coord2D_to_yaxis(this->m_ydata[i+1][j+1]);
                        glVertex2d(x, y);
                        x = this->coord2D_to_xaxis(this->m_xdata[i+1][j]);
                        y = this->coord2D_to_yaxis(this->m_ydata[i+1][j]);
                        glVertex2d(x, y);
                    glEnd();
                }
            }
        }
    }
}

// -*-
void SurfaceBase::draw3d(){
    std::unique_lock<std::mutex> lock(this->m_data_mtx);
    std::vector<float> rgb;
    auto ny = this->m_zdata.size();
    auto nx = this->m_zdata[0].size();

    if(ny == 1){
        if(this->m_faceColor!="none"){
            double x, y, z;
            for(auto i=0; i < ny-1; ++i){
                for(auto j=0; j < nx - 1; ++j){
                    auto color = Color(this->m_faceColor);
                    rgb = color.to_vector();
                    glColor3d(rgb[0], rgb[1], rgb[2]);
                    if(this->m_faceColor=="flat"){
                        rgb = this->m_cdata[i][j];
                        glColor3f(rgb[0], rgb[1], rgb[2]);
                    }
                    glBegin(GL_TRIANGLES);
                        x = this->coord3D_to_xaxis(this->m_xdata[0][j]);
                        y = this->coord3D_to_yaxis(this->m_ydata[0][i]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[0][j]);
                        y = this->coord3D_to_yaxis(this->m_ydata[0][i+1]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i+1][j]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[0][j+1]);
                        y = this->coord3D_to_yaxis(this->m_ydata[0][i+1]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i+1][j+1]);
                        glVertex3d(x, y, z);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                        x = this->coord3D_to_xaxis(this->m_xdata[0][j]);
                        y = this->coord3D_to_yaxis(this->m_ydata[0][i]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[0][j+1]);
                        y = this->coord3D_to_yaxis(this->m_ydata[0][i]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i][j+1]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[0][j+1]);
                        y = this->coord3D_to_yaxis(this->m_ydata[0][i+1]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i+1][j+1]);
                        glVertex3d(x, y, z);
                    glEnd();
                }
            }
        }

        if(this->m_edgeColor!="none"){
            glLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3d(rgb[0], rgb[1], rgb[2]);

            double x, y, z;
            for(auto i=0; i < ny; ++i){
                glBegin(GL_LINE_STRIP);
                for(auto j=0; j < nx; ++j){
                    x = this->coord3D_to_xaxis(this->m_xdata[0][j]);
                    y = this->coord3D_to_yaxis(this->m_ydata[0][i]);
                    z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                    glVertex3d(x, y, z);
                }
                glEnd();
            }
            for(auto j=0; j < nx; ++j){
                glBegin(GL_LINE_STRIP);
                for(auto i=0; i < ny; ++i){
                    x = this->coord3D_to_xaxis(this->m_xdata[0][j]);
                    y = this->coord3D_to_yaxis(this->m_ydata[0][i]);
                    z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                    glVertex3d(x, y, z);
                }
                glEnd();
            }
        }
    }else{
        if(this->m_faceColor!="none"){
            double x, y, z;
            for(auto i=0; i < ny-1; ++i){
                for(auto j=0; j < nx; ++j){
                    auto color = Color(this->m_faceColor);
                    rgb = color.to_vector();
                    glColor3d(rgb[0], rgb[1], rgb[2]);
                    if(this->m_faceColor=="flat"){
                        rgb = this->m_cdata[i][j];
                        glColor3f(rgb[0], rgb[1], rgb[2]);
                    }
                    glBegin(GL_TRIANGLES);
                        x = this->coord3D_to_xaxis(this->m_xdata[i][j]);
                        y = this->coord3D_to_yaxis(this->m_ydata[i][j]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[i][j+1]);
                        y = this->coord3D_to_yaxis(this->m_ydata[i][j+1]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i][j+1]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[i+1][j+1]);
                        y = this->coord3D_to_yaxis(this->m_ydata[i+1][j+1]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i+1][j+1]);
                        glVertex3d(x, y, z);
                    glEnd();

                    glBegin(GL_TRIANGLES);
                        x = this->coord3D_to_xaxis(this->m_xdata[i][j]);
                        y = this->coord3D_to_yaxis(this->m_ydata[i][j]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[i+1][j]);
                        y = this->coord3D_to_yaxis(this->m_ydata[i+1][j]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i+1][j]);
                        glVertex3d(x, y, z);
                        x = this->coord3D_to_xaxis(this->m_xdata[i+1][j+1]);
                        y = this->coord3D_to_yaxis(this->m_ydata[i+1][j+1]);
                        z = this->coord3D_to_zaxis(this->m_zdata[i+1][j+1]);
                        glVertex3d(x, y, z);
                    glEnd();
                }
            }
        }
        if(this->m_edgeColor != "none"){
            glLineWidth(this->m_lineWidth);
            auto color = Color(this->m_edgeColor);
            rgb = color.to_vector();
            glColor3d(rgb[0], rgb[1], rgb[2]);
            
            double x, y, z;
            for(auto i=0; i < ny; ++i){
                glBegin(GL_LINE_STRIP);
                for(auto j=0; j < nx; ++j){
                    x = this->coord3D_to_xaxis(this->m_xdata[i][j]);
                    y = this->coord3D_to_yaxis(this->m_ydata[i][j]);
                    z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                    glVertex3d(x, y, z);
                }
                glEnd();
            }
            for(auto j=0; j < nx; ++j){
                glBegin(GL_LINE_STRIP);
                for(auto i=0; i < ny; ++i){
                    x = this->coord3D_to_xaxis(this->m_xdata[i][j]);
                    y = this->coord3D_to_yaxis(this->m_ydata[i][j]);
                    z = this->coord3D_to_zaxis(this->m_zdata[i][j]);
                    glVertex3d(x, y, z);
                }
                glEnd();
            }
        }
    }
}

// -*-
void SurfaceBase::contourc(
    const Vector<double>& xvec, const Vector<double>& yvec,
    const Matrix<double>& zmat, const Vector<double>& values,
    Matrix<double>& cmat
){
    double x, y, z;
    auto ny = zmat.size();
    auto nx = zmat[0].size();
    Contour contour;                    // c
    std::vector<Contour> contourVec;    // vc
    std::deque<Contour> contourQueue;   // ac
    cmat.resize(2);

    for(auto iv =0; iv < values.size(); ++iv){
        z = values[iv];
        /* find contour points */
        contourVec.clear();
        for(auto i=0; i < ny; ++i){
            for(auto j=0; j < nx; ++j){
                bool ok = (j < nx-1) && ((zmat[i][j+1]-z)*(zmat[i][j]-z) < 0);
                if(ok){
                    x = (xvec[j]+(xvec[j+1]-xvec[j]))*(z-zmat[i][j])/(zmat[i][j+1]+zmat[i][i]);
                    contour.xj = j;
                    contour.yi = i;
                    contour.xy = 0;
                    contour.done = 0;
                    contour.x = x;
                    contour.y = yvec[i];
                    contourVec.push_back(contour);
                }
                ok = (
                    (i < ny - 1) && ((zmat[i+1][j]-z)*(zmat[i][j]-z) < 0)
                );
                if(ok){
                    y = yvec[i]+(yvec[i+1]-yvec[i])*(z-zmat[i][j])/(zmat[i+1][j]-zmat[i][j]);
                    contour.xj = j;
                    contour.yi = i;
                    contour.xy = 1;
                    contour.done = 0;
                    contour.x = xvec[j];
                    contour.y = y;
                    contourVec.push_back(contour);
                }
            }
        }
        /* sort contour points */
        int is = 0;
        unsigned int m, kk;
        int mode, nxtmode;
        mode = 0;
        while(mode < 5){
            if(mode == 0){
                contourQueue.clear();
                is = 0;
                m = 0;
                while(!is && (m < contourVec.size())){
                    if(!contourVec[m].done){
                        is = 1;
                        kk = m;
                    }
                    ++m;
                }
                if(is){
                    contourVec[kk].done = 2;
                    contour = contourVec[kk];
                    contourQueue.push_back(contour);
                    nxtmode = 1;
                }else{
                    nxtmode = 5;
                }
            }else if(mode==1 || mode==3){
                is = 0;
                m = 0;
                while(!is && (m < contourVec.size())){
                    is = 0;
                    if((!contourVec[m].done) || ((contourVec[m].done==2)&&(contourQueue.size()>2))){
                        bool yes = (
                            (contour.xy==0) && (contourVec[m].xy==0) &&
                            (contourVec[m].xj==contour.xj) &&
                            (contourVec[m].yi==contour.yi-1)
                        );
                        if(yes){ is = 1; }
                        yes = (
                            (contour.xy==0) && (contourVec[m].xy==0) &&
                            (contourVec[m].xj==contour.xj) &&
                            (contourVec[m].yi==contour.yi+1)
                        );
                        if(yes){ is = 2; }
                        yes = (
                            (contour.xy==0) && (contourVec[m].xy==1) &&
                            (contourVec[m].xj==contour.xj) &&
                            (contourVec[m].yi==contour.yi)
                        );
                        if(yes){ is = 3; }
                        yes = (
                            (contour.xy==0) && (contourVec[m].xy==1) &&
                            (contourVec[m].xj==contour.xj+1) &&
                            (contourVec[m].yi==contour.yi)
                        );
                        if(yes){ is = 4; }
                        yes = (
                            (contour.xy==0) && (contourVec[m].xy==1) &&
                            (contourVec[m].xj==contour.xj) &&
                            (contourVec[m].yi==contour.yi-1)
                        );
                        if(yes){ is = 5; }
                        yes = (
                            (contour.xy==0) && (contourVec[m].xy==1) &&
                            (contourVec[m].xj==contour.xj+1) &&
                            (contourVec[m].yi==contour.yi-1)
                        );
                        if(yes){ is = 6; }
                        yes = (
                            (contour.xy==1) && (contourVec[m].xy==1) &&
                            (contourVec[m].xj==contour.xj+1) &&
                            (contourVec[m].yi==contour.yi)
                        );
                        if(yes){ is = 7; }
                        yes = (
                            (contour.xy==1) && (contourVec[m].xy==1) &&
                            (contourVec[m].xj==contour.xj-1) &&
                            (contourVec[m].yi==contour.yi)
                        );
                        if(yes){ is = 8; }
                        yes = (
                            (contour.xy==1) && (contourVec[m].xy==0) &&
                            (contourVec[m].xj==contour.xj) &&
                            (contourVec[m].yi==contour.yi)
                        );
                        if(yes){ is = 9; }
                        yes = (
                            (contour.xy==1) && (contourVec[m].xy==0) &&
                            (contourVec[m].xj==contour.xj) &&
                            (contourVec[m].yi==contour.yi+1)
                        );
                        if(yes){ is = 10; }
                        yes = (
                            (contour.xy==1) && (contourVec[m].xy==0) &&
                            (contourVec[m].xj==contour.xj-1) &&
                            (contourVec[m].yi==contour.yi)
                        );
                        if(yes){ is = 11; }
                        yes = (
                            (contour.xy==1) && (contourVec[m].xy==0) &&
                            (contourVec[m].xj==contour.xj-1) &&
                            (contourVec[m].yi==contour.yi+1)
                        );
                        if(yes){ is = 12; }
                    }
                    if(is){ kk = m;}
                    m++;
                }
                if(is){
                    contourVec[kk].done = 1;
                    contour = contourVec[kk];
                }
                if(mode==1){
                    if(is){
                        contourQueue.push_back(contourVec[kk]);
                        nxtmode = 1;
                    }else{
                        nxtmode = 2;
                    }
                }else if(mode==3){
                    if(is){
                        contourQueue.push_front(contourVec[kk]);
                        nxtmode = 3;
                    }else{
                        nxtmode = 4;
                    }
                }
            }else if(mode==2){
                contour = contourQueue[0];
                nxtmode = 3;
            }else if(mode==4){
                if(contourQueue.size()){
                    cmat[0].push_back(z);
                    cmat[1].push_back(contourQueue.size());
                    for(auto i=0; i < contourQueue.size(); ++i){
                        cmat[0].push_back(contourQueue[i].x);
                        cmat[1].push_back(contourQueue[i].y);
                    }
                }
                nxtmode = 0;
            }
            mode = nxtmode;
        }
    }
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-