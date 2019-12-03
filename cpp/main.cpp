#include <gtk/gtk.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include <iostream>
#include <string>
#include "src/cv2ImageProcessing.h"

using namespace cv;
using namespace std;
cv2ImageProcessing cv2ip;

char* comb_char(char* ch1, char* ch2);

/*Signal handler for the "clicked" signal of the EQ RadioButton*/
static void eq_rgb_tog_clicked (GtkWidget *button, gpointer   user_data);
static void eq_rgb_ind_clicked (GtkWidget *button, gpointer   user_data);
static void eq_hsv_clicked (GtkWidget *button, gpointer   user_data);
static void eq_yuv_clicked (GtkWidget *button, gpointer   user_data);

/*Signal handler for the "clicked" signal of the Match RadioButton*/
static void match_rgb_tog_clicked (GtkWidget *button, gpointer   user_data);
static void match_rgb_ind_clicked (GtkWidget *button, gpointer   user_data);
static void match_hsv_clicked (GtkWidget *button, gpointer   user_data);
static void match_v_clicked (GtkWidget *button, gpointer   user_data);
static void match_yuv_clicked (GtkWidget *button, gpointer   user_data);
static void match_y_clicked (GtkWidget *button, gpointer   user_data);

// Signal handler for Eq & Match
static void hist_equalize (GtkWidget *widget, gpointer data);
static void hist_match (GtkWidget *widget, gpointer data);

// Signal handler for Entry and init image
static void orig_enter_callback( GtkWidget *widget, GtkWidget *entry );
static void ref_enter_callback( GtkWidget *widget, GtkWidget *entry );

GError** err = NULL;

// filename
char img_dir[]="./img/output/";
char pdf_dir[]="./img/pdf/pdf_";
char cdf_dir[]="./img/cdf/cdf_";
char orig_name[]="";
char ref_name[]="";
char eq_name[]="eq.jpg";
char match_name[]="match.jpg";

CvImage img_orig, img_ref, img_eq, img_match, img_yuv, img_hsv, img_ref_hsv, img_ref_yuv;

// gtk widgets
GtkWidget *window;
GtkWidget *grid, *boxEq, *boxMatch;
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


int main (int   argc, char *argv[])
{

// =================================================
//                  window
// =================================================
  gtk_init (&argc, &argv);

  /* create a new window, and set its title */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Grid");
  gtk_widget_set_size_request (window, 100,100);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 20);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);

// =================================================
//                  Radio Buttons
// =================================================

//For Equalization
  boxEq = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_grid_attach (GTK_GRID (grid), boxEq, 2, 1, 1, 2);
  
  radioBtnEq = gtk_radio_button_new_with_label (NULL,"RGB-ALL");
  radioEqRGB = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnEq), "RGB-IND");
  radioEqHSV = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnEq), "HSV-V");
  radioEqYUV = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnEq), "YUV-Y");

  gtk_box_pack_start (GTK_BOX (boxEq), radioBtnEq,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxEq), radioEqRGB,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxEq), radioEqHSV,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxEq), radioEqYUV,1,0,0);

  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (radioBtnEq), "clicked", G_CALLBACK (eq_rgb_tog_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioEqRGB), "clicked", G_CALLBACK (eq_rgb_ind_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioEqHSV), "clicked", G_CALLBACK (eq_hsv_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioEqYUV), "clicked", G_CALLBACK (eq_yuv_clicked), NULL);
  

//For Match
  boxMatch = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_grid_attach (GTK_GRID (grid), boxMatch, 2, 4, 1, 2);

  radioBtnMatch=gtk_radio_button_new_with_label (NULL,"RGB-ALL");
  radioMatchRGB = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnMatch), "RGB-IND");
  radioMatchHSV = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnMatch), "HSV");
  radioMatchV   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnMatch), "HSV-V");
  radioMatchYUV = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnMatch), "YUV");
  radioMatchY   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radioBtnMatch), "YUV-Y");
  
  gtk_box_pack_start (GTK_BOX (boxMatch), radioBtnMatch,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxMatch), radioMatchRGB,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxMatch), radioMatchHSV,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxMatch), radioMatchV,  1,0,0);
  gtk_box_pack_start (GTK_BOX (boxMatch), radioMatchYUV,1,0,0);
  gtk_box_pack_start (GTK_BOX (boxMatch), radioMatchY,  1,0,0);
  
  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (radioBtnMatch), "clicked", G_CALLBACK (match_rgb_tog_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioMatchRGB), "clicked", G_CALLBACK (match_rgb_ind_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioMatchHSV), "clicked", G_CALLBACK (match_hsv_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioMatchV),   "clicked", G_CALLBACK (match_v_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioMatchYUV), "clicked", G_CALLBACK (match_yuv_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (radioMatchY),   "clicked", G_CALLBACK (match_y_clicked), NULL);

// =================================================
//                     Buttons
// =================================================
  
// eq button
  button_eq = gtk_button_new_with_label ("直方圖等化");
  g_signal_connect (button_eq, "clicked", G_CALLBACK (hist_equalize), NULL);
  /* Place in (0, 0), fill 1 cell horizontally and vertically (ie no spanning)*/
  gtk_grid_attach (GTK_GRID (grid), button_eq, 3, 1, 1, 2);

// match button
  button_match = gtk_button_new_with_label ("直方圖匹配");
  g_signal_connect (button_match, "clicked", G_CALLBACK (hist_match), NULL);
  gtk_grid_attach (GTK_GRID (grid), button_match, 3, 4, 1, 2);

// quit button
  button_quit = gtk_button_new_with_label ("Quit");
  g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
  gtk_grid_attach (GTK_GRID (grid), button_quit, 0, 6, 6, 1);

// =================================================
//                  Entry & Label
// =================================================
  orig_label = gtk_label_new ("原圖檔：");
  gtk_grid_attach (GTK_GRID (grid), orig_label, 0, 0, 1, 1);
  orig_entry = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (orig_entry), 50);
  g_signal_connect (orig_entry, "activate",
        G_CALLBACK (orig_enter_callback),
        orig_entry);
  gtk_grid_attach (GTK_GRID (grid), orig_entry, 1, 0, 5, 1);

  ref_label = gtk_label_new ("參考圖檔：");
  gtk_grid_attach (GTK_GRID (grid), ref_label, 0, 3, 1, 1);
  ref_entry = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (ref_entry), 50);
  g_signal_connect (ref_entry, "activate",
        G_CALLBACK (ref_enter_callback),
        ref_entry);
  gtk_grid_attach (GTK_GRID (grid), ref_entry, 1, 3, 5, 1);

// =================================================
//                     Init Images
// =================================================

// original image
  orig_widg.image  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid), orig_widg.image, 0, 1, 1, 2);

  orig_widg.pdf_img = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid), orig_widg.pdf_img, 1, 1, 1, 1);
  
  orig_widg.cdf_img = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid), orig_widg.cdf_img, 1, 2, 1, 1);
  
// reference image
  ref_widg.image=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid), ref_widg.image, 0, 4, 1, 2);

  ref_widg.pdf_img =gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid), ref_widg.pdf_img, 1, 4, 1, 1);

  ref_widg.cdf_img =gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid), ref_widg.cdf_img, 1, 5, 1, 1);

// hist_equalize output image
  eq_widg.image=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid), eq_widg.image, 4, 1, 1, 2);
  
  eq_widg.pdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid), eq_widg.pdf_img, 5, 1, 1, 1);
  
  eq_widg.cdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid), eq_widg.cdf_img, 5, 2, 1, 1);

// histmatch output image
  match_widg.image=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid), match_widg.image, 4, 4, 1, 2);
  
  match_widg.pdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid), match_widg.pdf_img, 5, 4, 1, 1);
  
  match_widg.cdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid), match_widg.cdf_img, 5, 5, 1, 1);

// show
  gtk_widget_show_all (window);
  gtk_main ();

  return 0;
}


// =================================================
//                Callback functions
// =================================================
/*Signal handler for the "toggled" signal of the RadioButton*/
static void eq_rgb_tog_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_RGB_TOG;
}
static void eq_rgb_ind_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_RGB_IND;
}
static void eq_hsv_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_HSV;
}
static void eq_yuv_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_YUV;
}


/*Signal handler for the "toggled" signal of the RadioButton*/
static void match_rgb_tog_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_RGB_TOG;
}
static void match_rgb_ind_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_RGB_IND;
}
static void match_hsv_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_HSV;
}
static void match_v_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_V;
}
static void match_yuv_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_YUV;
}
static void match_y_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_Y;
}


static void hist_equalize (GtkWidget *widget, gpointer data)
{
    switch (eq_type)
    {
    case EQ_RGB_TOG:
      cv2ip.AllChEqualize(img_eq,img_orig);
      break;
    case EQ_RGB_IND:
      cv2ip.ColorEqualize(img_eq,img_orig);
      break;
    case EQ_HSV:
      cvtColor(img_orig,img_hsv,COLOR_BGR2HSV);
      cv2ip.ColorEqualize(img_eq,img_hsv,cv2ip.USE_HSV);
      cvtColor(img_eq,img_eq,COLOR_HSV2BGR);
      break;
    case EQ_YUV:
      cvtColor(img_orig,img_yuv,COLOR_BGR2YUV);
      cv2ip.ColorEqualize(img_eq,img_yuv,cv2ip.USE_YUV);
      cvtColor(img_eq,img_eq,COLOR_YUV2BGR);
      break;
    default:
      break;
    }
    cv2ip.ShowCDF(img_eq,eq_widg.pdf_cvImg,eq_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(img_dir,eq_name),img_eq);
    cv2ip.ImWrite(comb_char(pdf_dir,eq_name),eq_widg.pdf_cvImg);
    cv2ip.ImWrite(comb_char(cdf_dir,eq_name),eq_widg.cdf_cvImg);
    eq_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,eq_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(eq_widg.image),eq_widg.pixbuf);
    eq_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,eq_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(eq_widg.pdf_img),eq_widg.pdf_pix);
    eq_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,eq_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(eq_widg.cdf_img),eq_widg.cdf_pix);
}

static void hist_match (GtkWidget *widget, gpointer data)
{
    switch (match_type)
    {
    case MAT_RGB_TOG:
      cv2ip.HistMatchAll(img_match,img_orig,img_ref);
      break;
    case MAT_RGB_IND:
      cv2ip.HistMatching(img_match,img_orig,img_ref);
      break;
    case MAT_HSV:
      cvtColor(img_orig,img_hsv,COLOR_BGR2HSV);
      cvtColor(img_ref,img_ref_hsv,COLOR_BGR2HSV);
      cv2ip.HistMatching(img_match,img_hsv,img_ref_hsv,cv2ip.USE_RGB);
      cvtColor(img_match,img_match,COLOR_HSV2BGR);
      break;
    case MAT_V:
      cvtColor(img_orig,img_hsv,COLOR_BGR2HSV);
      cvtColor(img_ref,img_ref_hsv,COLOR_BGR2HSV);
      cv2ip.HistMatching(img_match,img_hsv,img_ref_hsv,cv2ip.USE_HSV);
      cvtColor(img_match,img_match,COLOR_HSV2BGR);
      break;
    case MAT_YUV:
      cvtColor(img_orig,img_yuv,COLOR_BGR2YUV);
      cvtColor(img_ref,img_ref_yuv,COLOR_BGR2YUV);
      cv2ip.HistMatching(img_match,img_yuv,img_ref_yuv,cv2ip.USE_RGB);
      cvtColor(img_match,img_match,COLOR_YUV2BGR);
      break;
    case MAT_Y:
      cvtColor(img_orig,img_yuv,COLOR_BGR2YUV);
      cvtColor(img_ref,img_ref_yuv,COLOR_BGR2YUV);
      cv2ip.HistMatching(img_match,img_yuv,img_ref_yuv,cv2ip.USE_YUV);
      cvtColor(img_match,img_match,COLOR_YUV2BGR);
      break;
    
    default:
      break;
    }
    
    cv2ip.ShowCDF(img_match,match_widg.pdf_cvImg,match_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(img_dir,match_name),img_match);
    cv2ip.ImWrite(comb_char(pdf_dir,match_name),match_widg.pdf_cvImg);
    cv2ip.ImWrite(comb_char(cdf_dir,match_name),match_widg.cdf_cvImg);
    match_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,match_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(match_widg.image),match_widg.pixbuf);
    match_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,match_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(match_widg.pdf_img),match_widg.pdf_pix);
    match_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,match_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(match_widg.cdf_img),match_widg.cdf_pix);
}

static void orig_enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(orig_name,entry_text);
  img_orig=cv2ip.ImRead(orig_name);
  orig_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(orig_name,800,400,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.image),orig_widg.pixbuf);
  cv2ip.ShowCDF(img_orig,orig_widg.pdf_cvImg,orig_widg.cdf_cvImg);
  cv2ip.ImWrite(comb_char(pdf_dir,orig_name),orig_widg.pdf_cvImg);
  cv2ip.ImWrite(comb_char(cdf_dir,orig_name),orig_widg.cdf_cvImg);
  orig_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,orig_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.pdf_img),orig_widg.pdf_pix);
  orig_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,orig_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.cdf_img),orig_widg.cdf_pix);
  
}
static void ref_enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(ref_name,entry_text);
  img_ref=cv2ip.ImRead(ref_name);
  ref_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(ref_name,800,400,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.image),ref_widg.pixbuf);
  cv2ip.ShowCDF(img_ref,ref_widg.pdf_cvImg,ref_widg.cdf_cvImg);
  cv2ip.ImWrite(comb_char(pdf_dir,ref_name),ref_widg.pdf_cvImg);
  cv2ip.ImWrite(comb_char(cdf_dir,ref_name),ref_widg.cdf_cvImg);
  ref_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,ref_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.pdf_img),ref_widg.pdf_pix);
  ref_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,ref_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.cdf_img),ref_widg.cdf_pix);
}


// =================================================
//                custom func
// =================================================
char* comb_char(char* ch1, char* ch2)
{
  char* ret_ch=new char[strlen(ch1)+strlen(ch2)+1];// how to delete it after return?
  strcpy(ret_ch,ch1);
  strcat(ret_ch,ch2);
  return ret_ch;
}