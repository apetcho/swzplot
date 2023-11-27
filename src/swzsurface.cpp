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


// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-