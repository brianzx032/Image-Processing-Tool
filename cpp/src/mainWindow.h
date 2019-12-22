#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <gtk/gtk.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include <iostream>
#include <string>
#include "cv2ImageProcessing.h"

using namespace cv;
using namespace std;

/******************************************************************
 ******************************************************************
 **                                                              **
 **                                                              **
 **                     Not Yet Finished                         **
 **                                                              **
 **                                                              **
 ******************************************************************
 ******************************************************************/
class mainWindow
{
    public:

/* -------------------------------------------------*
 *            Constructor & Deconstructor           *
 * -------------------------------------------------*/
    mainWindow(void);
    ~mainWindow(void);

    void windowInit(int   argc, char *argv[]);

/* -------------------------------------------------*
 *                    custom func                   *
 * -------------------------------------------------*/
    char* comb_char(char* ch1, char* ch2);

    CvImage img_orig, img_ref, img_eq, img_match, img_yuv, img_hsv, img_ref_hsv, img_ref_yuv;
    
    // gtk widgets
    GtkWidget *window;
    GtkWidget *grid_a1,*stack_pages,*grid_a3,*stack_sw,*grid_stack;
    GtkWidget *grid_a2, *boxEq, *boxMatch;
    GtkWidget *button_quit, *button_eq, *button_match;
    GtkWidget *radioBtnEq, *radioEqHSV, *radioEqYUV, *radioEqRGB;
    GtkWidget *radioBtnMatch, *radioMatchRGB, *radioMatchHSV, *radioMatchYUV, *radioMatchV, *radioMatchY;
    GtkWidget *orig_entry, *ref_entry, *orig_label, *ref_label;

    // my image struct
    struct imgWidget
    {
    GdkPixbuf *pixbuf, *pdf_pix, *cdf_pix;
    GtkWidget *image, *pdf_img, *cdf_img;
    CvImage cvImg, pdf_cvImg, cdf_cvImg;
    } orig_widg, ref_widg, eq_widg, match_widg;


    // enum
    enum EQTYPE {EQ_RGB_TOG=0, EQ_RGB_IND, EQ_HSV, EQ_YUV};
    enum MATCHTYPE {MAT_RGB_TOG=0, MAT_RGB_IND, MAT_HSV, MAT_V, MAT_YUV, MAT_Y};
    int eq_type=EQ_RGB_TOG;
    int match_type=MAT_RGB_TOG;

    /*Signal handler for the "clicked" signal of the EQ RadioButton*/
     void eq_rgb_tog_clicked (GtkWidget *button, gpointer   user_data);
     void eq_rgb_ind_clicked (GtkWidget *button, gpointer   user_data);
     void eq_hsv_clicked (GtkWidget *button, gpointer   user_data);
     void eq_yuv_clicked (GtkWidget *button, gpointer   user_data);

    /*Signal handler for the "clicked" signal of the Match RadioButton*/
     void match_rgb_tog_clicked (GtkWidget *button, gpointer   user_data);
     void match_rgb_ind_clicked (GtkWidget *button, gpointer   user_data);
     void match_hsv_clicked (GtkWidget *button, gpointer   user_data);
     void match_v_clicked (GtkWidget *button, gpointer   user_data);
     void match_yuv_clicked (GtkWidget *button, gpointer   user_data);
     void match_y_clicked (GtkWidget *button, gpointer   user_data);

    // Signal handler for Eq & Match
     void hist_equalize (GtkWidget *widget, gpointer data);
     void hist_match (GtkWidget *widget, gpointer data);

    // Signal handler for Entry and init image
     void orig_enter_callback( GtkWidget *widget, GtkWidget *entry );
     void ref_enter_callback( GtkWidget *widget, GtkWidget *entry );


    cv2ImageProcessing cv2ip;

    GError** err = NULL;

    // filename
    char src_dir[];
    char img_dir[];
    char pdf_dir[];
    char cdf_dir[];
    char orig_name[];
    char ref_name[];
    char eq_name[];
    char match_name[];

    private:
};

#endif