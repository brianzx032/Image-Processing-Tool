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
static void eq_rgb_tog_clicked (GtkWidget *button, gpointer   data);
static void eq_rgb_ind_clicked (GtkWidget *button, gpointer   data);
static void eq_hsv_clicked (GtkWidget *button, gpointer   data);
static void eq_yuv_clicked (GtkWidget *button, gpointer   data);

/*Signal handler for the "clicked" signal of the Match RadioButton*/
static void match_rgb_tog_clicked (GtkWidget *button, gpointer   data);
static void match_rgb_ind_clicked (GtkWidget *button, gpointer   data);
static void match_hsv_clicked (GtkWidget *button, gpointer   data);
static void match_v_clicked (GtkWidget *button, gpointer   data);
static void match_yuv_clicked (GtkWidget *button, gpointer   data);
static void match_y_clicked (GtkWidget *button, gpointer   data);

/*Signal handler for the "clicked" signal of the Smooth RadioButton*/
static void sm_blur_clicked (GtkWidget *button, gpointer   data);
static void sm_box_clicked (GtkWidget *button, gpointer   data);
static void sm_gaussian_clicked (GtkWidget *button, gpointer   data);
static void sm_median_clicked (GtkWidget *button, gpointer   data);
static void sm_bilateral_clicked (GtkWidget *button, gpointer   data);

/*Signal handler for the "clicked" signal of the EdgeDetect RadioButton*/
static void edge_sobel_clicked (GtkWidget *button, gpointer   data);
static void edge_canny_clicked (GtkWidget *button, gpointer   data);
static void edge_scharr_clicked (GtkWidget *button, gpointer   data);
static void edge_laplace_clicked (GtkWidget *button, gpointer   data);
static void edge_roberts_clicked (GtkWidget *button, gpointer   data);
static void edge_prewitt_clicked (GtkWidget *button, gpointer   data);
static void edge_kirsch_clicked (GtkWidget *button, gpointer   data);

/*Signal handler for the "clicked" signal of the sharpen RadioButton*/
static void shp_lap1_clicked (GtkWidget *button, gpointer   data);
static void shp_lap2_clicked (GtkWidget *button, gpointer   data);
static void shp_seclog_clicked (GtkWidget *button, gpointer   data);
static void shp_unsharp_clicked (GtkWidget *button, gpointer   data);

/* Signal handler for Eq & Match */
static void hist_equalize (GtkWidget *widget, gpointer data);
static void hist_match (GtkWidget *widget, gpointer data);

static void img_smooth (GtkWidget *widget, gpointer data);
static void edge_detect (GtkWidget *widget, gpointer data);
static void img_sharpen (GtkWidget *widget, gpointer data);

static void ksize_changed (GtkWidget *widget, gpointer data);
static void canny_thrd1_changed (GtkWidget *widget, gpointer data);
static void canny_thrd2_changed (GtkWidget *widget, gpointer data);
static void landa_changed (GtkWidget *widget, gpointer data);

static void diff_gain_changed (GtkWidget *widget, gpointer data);

static void width_changed (GtkWidget *widget, gpointer data);
static void height_changed (GtkWidget *widget, gpointer data);
static void x_changed (GtkWidget *widget, gpointer data);
static void y_changed (GtkWidget *widget, gpointer data);

static void blend_clicked (GtkWidget *widget, gpointer data);

static void load_in (GtkWidget *widget, gpointer data);

/* Signal handler for Entry and init image */
static void orig_enter_callback( GtkWidget *widget, GtkWidget *entry );
static void ref_enter_callback( GtkWidget *widget, GtkWidget *entry );
static void fore_enter_callback( GtkWidget *widget, GtkWidget *entry );

static void show_diff();
static void orig_init(char *filename);
static void ref_init();
static void fore_init();
static void blend_img();


GError** err = NULL;

/* filename */
char src_dir[]="./img/src/";
char img_dir[]="./img/output/";
char pdf_dir[]="./img/pdf/pdf_";
char cdf_dir[]="./img/cdf/cdf_";
char diff_dir[]="./img/diff/";
char orig_name[]="logo.png";
char ref_name[]="logo.png";
char fore_name[]="logo.png";
char eq_name[]="eq.jpg";
char match_name[]="match.jpg";
char smooth_name[]="smooth.jpg";
char edge_name[]="edge.jpg";
char sharpen_name[]="sharpen.jpg";
char diff_name[]="diff.jpg";
char blend_name[]="blend.jpg";
char output_name[]="output.jpg";


/* window widgets */
GtkWidget *window;

/* grid widgets */
GtkWidget *grid_stack;
GtkWidget *grid_a1,*grid_a2,*grid_a3;

/* stack widgets */
GtkWidget *stack_pages,*stack_sw;

/* box widgets */
GtkWidget *box_eq, *box_match;
GtkWidget *box_smooth, *box_edge, *box_sharpen;

/* scale & adjustment widgets */
GtkWidget *scale_ksize, *scale_canny_thrd1, *scale_canny_thrd2, *scale_landa, *scale_diff_gain;
GtkAdjustment *adj_ksize, *adj_canny_thrd1, *adj_canny_thrd2, *adj_landa, *adj_diff_gain;
GtkWidget *scale_width, *scale_height, *scale_x, *scale_y;
GtkAdjustment *adj_width, *adj_height, *adj_x, *adj_y;

/* button widgets */
GtkWidget *button_quit, *button_eq, *button_match, *button_sm, *button_edge, *button_sharpen, *button_blend, *button_load_in;

/* radiobuttons widgets */
GtkWidget *rbtn_eq_rgb_a, *rbtn_eq_rgb_i, *rbtn_eq_hsv, *rbtn_eq_yuv;
GtkWidget *rbtn_mat_rgb_a, *rbtn_mat_rgb_i, *rbtn_mat_hsv, *rbtn_mat_yuv, *rbtn_mat_s_v, *rbtn_mat_s_y;
GtkWidget *rbtn_smt_blur, *rbtn_smt_box, *rbtn_smt_gaus, *rbtn_smt_medi, *rbtn_smt_bila;
GtkWidget *rbtn_edg_sob, *rbtn_edg_can, *rbtn_edg_sch, *rbtn_edg_lap, *rbtn_edg_rob, *rbtn_edg_pre, *rbtn_edg_kir;
GtkWidget *rbtn_shp_lap1, *rbtn_shp_lap2, *rbtn_shp_log, *rbtn_shp_uns;

/* entry widgets */
GtkWidget *entry_orig, *entry_ref, *entry_fore;

/* label widgets */
GtkWidget *lb_ksize, *lb_can_thrd1, *lb_can_thrd2, *lb_landa;
GtkWidget *lb_smooth, *lb_edge, *lb_sharpen, *lb_gain_diff;
GtkWidget *lb_height, *lb_width, *lb_x, *lb_y;

/* my image struct */
struct imgWidget
{
  GtkWidget *label;
  GdkPixbuf *pixbuf, *pdf_pix, *cdf_pix;
  GtkWidget *a1_img, *a2_img, *a3_img, *pdf_img, *cdf_img;
  CvImage pic_cvImg, pdf_cvImg, cdf_cvImg, yuv_cvImg, hsv_cvImg;
} orig_widg, ref_widg, eq_widg, match_widg, output_widg, diff_widg, fore_widg, blend_widg;


/* enum */
enum EQTYPE {EQ_RGB_TOG=0, EQ_RGB_IND, EQ_HSV, EQ_YUV};
enum MATCHTYPE {MAT_RGB_TOG=0, MAT_RGB_IND, MAT_HSV, MAT_V, MAT_YUV, MAT_Y};
int eq_type=EQ_RGB_TOG;
int match_type=MAT_RGB_TOG;
cv2ImageProcessing::CV2_IMSMOOTH_TYPE smooth_type;
cv2ImageProcessing::CV2_EDGEDETECT_TYPE edge_detect_type;
cv2ImageProcessing::CV2_SHARPENING_TYPE sharpen_type;

int orig_size_min=0;
double diff_gain=1;
int width=10,height=10,x=0,y=0;

int main (int   argc, char *argv[])
{
/******************************************************************
 ******************************************************************
 **                                                              **
 **                                                              **
 **                          main window                         **
 **                                                              **
 **                                                              **
 ******************************************************************
 ******************************************************************/


/* -------------------------------------------------*
 *                      window                      *
 * -------------------------------------------------*/
  gtk_init (&argc, &argv);

  /* create a new window, and set its title */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Cv2ImageProcessing");
  gtk_widget_set_size_request (window, 100,100);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 20);


/* -------------------------------------------------*
 *                        grid                      *
 * -------------------------------------------------*/
/* Here we construct the container that is going pack our pages */
  grid_stack = gtk_grid_new ();
  gtk_grid_set_row_spacing(GTK_GRID(grid_stack),10);
  gtk_grid_set_column_spacing(GTK_GRID(grid_stack),10);

  grid_a1 = gtk_grid_new ();
  gtk_grid_set_row_spacing(GTK_GRID(grid_a1),10);
  gtk_grid_set_column_spacing(GTK_GRID(grid_a1),20);

  grid_a2 = gtk_grid_new ();
  gtk_grid_set_column_spacing(GTK_GRID(grid_a2),10);
  
  grid_a3 = gtk_grid_new ();
  gtk_grid_set_row_spacing(GTK_GRID(grid_a3),10);
  gtk_grid_set_column_spacing(GTK_GRID(grid_a3),10);
  

/* -------------------------------------------------*
 *                        stack                     *
 * -------------------------------------------------*/
  stack_pages=gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(stack_pages),GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
  gtk_stack_set_transition_duration(GTK_STACK(stack_pages),500);
  
  gtk_stack_add_titled (GTK_STACK(stack_pages), GTK_WIDGET(grid_a1), "a1_nm", "影像混合顯示");
  gtk_stack_add_titled (GTK_STACK(stack_pages), GTK_WIDGET(grid_a2), "a2_nm", "直方圖等化 & 直方圖匹配");
  gtk_stack_add_titled (GTK_STACK(stack_pages), GTK_WIDGET(grid_a3), "a3_nm", "平滑處理 & 邊緣偵測 & 銳利化");

  stack_sw=gtk_stack_switcher_new();
  gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_sw),GTK_STACK(stack_pages));

  gtk_container_add (GTK_CONTAINER (window), grid_stack);
  gtk_grid_attach (GTK_GRID (grid_stack), stack_sw, 0, 0, 6, 1);
  gtk_grid_attach (GTK_GRID (grid_stack), stack_pages, 0, 2, 6, 1);


/* -------------------------------------------------*
 *                  Entry & Label                   *
 * -------------------------------------------------*/
  orig_widg.label = gtk_label_new ("原圖像：");
  gtk_grid_attach (GTK_GRID (grid_stack), orig_widg.label, 0, 1, 1, 1);
  entry_orig = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (entry_orig), 50);
  g_signal_connect (entry_orig, "activate",
        G_CALLBACK (orig_enter_callback),
        entry_orig);
  gtk_grid_attach (GTK_GRID (grid_stack), entry_orig, 1, 1, 4, 1);


/* -------------------------------------------------*
 *                      Buttons                     *
 * -------------------------------------------------*/
/* load in button */
  button_load_in = gtk_button_new_with_label ("導入前次結果");
  g_signal_connect (button_load_in, "clicked", G_CALLBACK (load_in), NULL);
  gtk_grid_attach (GTK_GRID (grid_stack), button_load_in, 5, 1, 1, 1);

/* quit button */
  button_quit = gtk_button_new_with_label ("Quit");
  g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
  gtk_grid_attach (GTK_GRID (grid_stack), button_quit, 0, 3, 6, 1);


/* -------------------------------------------------*
 *                       images                     *
 * -------------------------------------------------*/
  orig_widg.pixbuf  = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);
  orig_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,orig_name),200,200,err);
  orig_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,orig_name),200,200,err); 

  fore_widg.pixbuf  = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,fore_name),800,400,err);
  blend_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);
  
  ref_widg.pixbuf  = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,ref_name),800,400,err);
  ref_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,ref_name),200,200,err);
  ref_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,ref_name),200,200,err);

  eq_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);
  eq_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,orig_name),200,200,err);
  eq_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,orig_name),200,200,err);

  match_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);
  match_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,orig_name),200,200,err);
  match_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,orig_name),200,200,err);
  
  output_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);
  diff_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);


/******************************************************************
 ******************************************************************
 **                                                              **
 **                                                              **
 **                         assignment 1                         **
 **                                                              **
 **                                                              **
 ******************************************************************
 ******************************************************************/

/* -------------------------------------------------*
 *                    Init Images                   *
 * -------------------------------------------------*/
/* original image */
  orig_widg.a1_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a1), orig_widg.a1_img, 0, 0, 2, 5);

/* fore image */
  fore_widg.a1_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(fore_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a1), fore_widg.a1_img, 0, 6, 2, 1);
  fore_init();

/* original image */
  blend_widg.a1_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(blend_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a1), blend_widg.a1_img, 2, 6, 4, 1);

/* -------------------------------------------------*
 *                  Entry & Labels                  *
 * -------------------------------------------------*/
  fore_widg.label = gtk_label_new ("前景圖像：");
  gtk_grid_attach (GTK_GRID (grid_a1), fore_widg.label, 0, 5, 1, 1);
  entry_fore = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (entry_fore), 50);
  g_signal_connect (entry_fore, "activate",
        G_CALLBACK (fore_enter_callback),
        entry_fore);
  gtk_grid_attach (GTK_GRID (grid_a1), entry_fore, 1, 5, 1, 1);
  
  blend_widg.label = gtk_label_new ("輸出圖像");
  gtk_grid_attach (GTK_GRID (grid_a1), blend_widg.label, 2, 5, 4, 1);
  
  lb_width = gtk_label_new ("前景寬度（%）");
  gtk_grid_attach (GTK_GRID (grid_a1), lb_width, 2, 0, 1, 1);

  lb_height = gtk_label_new ("前景高度（%）");
  gtk_grid_attach (GTK_GRID (grid_a1), lb_height, 3, 0, 1, 1);
  
  lb_x = gtk_label_new ("前景x坐標（%）");
  gtk_grid_attach (GTK_GRID (grid_a1), lb_x, 4, 0, 1, 1);
  
  lb_y = gtk_label_new ("前景y坐標（%）");
  gtk_grid_attach (GTK_GRID (grid_a1), lb_y, 5, 0, 1, 1);


/* -------------------------------------------------*
 *                      Scales                      *
 * -------------------------------------------------*/
  adj_width=gtk_adjustment_new(width,0,100,1,1,0);
  scale_width=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_width));
  g_signal_connect (scale_width, "value-changed", G_CALLBACK (width_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a1), scale_width, 2, 1, 1, 3);

  adj_height=gtk_adjustment_new(height,0,100,1,1,0);
  scale_height=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_height));
  g_signal_connect (scale_height, "value-changed", G_CALLBACK (height_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a1), scale_height, 3, 1, 1, 3);

  adj_x=gtk_adjustment_new(x,0,100,1,1,0);
  scale_x=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_x));
  g_signal_connect (scale_x, "value-changed", G_CALLBACK (x_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a1), scale_x, 4, 1, 1, 3);

  adj_y=gtk_adjustment_new(y,0,100,1,1,0);
  scale_y=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_y));
  g_signal_connect (scale_y, "value-changed", G_CALLBACK (y_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a1), scale_y, 5, 1, 1, 3);


/* -------------------------------------------------*
 *                      Buttons                     *
 * -------------------------------------------------*/
  // blend button
  button_blend = gtk_button_new_with_label ("影像混合顯示");
  g_signal_connect (button_blend, "clicked", G_CALLBACK (blend_clicked), NULL);
  /* Place in (0, 0), fill 1 cell horizontally and vertically (ie no spanning)*/
  gtk_grid_attach (GTK_GRID (grid_a1), button_blend, 2, 4, 4, 1);


/******************************************************************
 ******************************************************************
 **                                                              **
 **                                                              **
 **                         assignment 2                         **
 **                                                              **
 **                                                              **
 ******************************************************************
 ******************************************************************/


/* -------------------------------------------------*
 *                  Radio Buttons                   *
 * -------------------------------------------------*/

//For Equalization
  box_eq = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_grid_attach (GTK_GRID (grid_a2), box_eq, 2, 1, 1, 2);
  
  rbtn_eq_rgb_a = gtk_radio_button_new_with_label (NULL,"RGB-ALL");
  rbtn_eq_rgb_i = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_eq_rgb_a), "RGB-IND");
  rbtn_eq_hsv   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_eq_rgb_a), "HSV-V");
  rbtn_eq_yuv   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_eq_rgb_a), "YUV-Y");

  gtk_box_pack_start (GTK_BOX (box_eq), rbtn_eq_rgb_a,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_eq), rbtn_eq_rgb_i,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_eq), rbtn_eq_hsv,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_eq), rbtn_eq_yuv,1,0,0);

  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_eq_rgb_a), "clicked", G_CALLBACK (eq_rgb_tog_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_eq_rgb_i), "clicked", G_CALLBACK (eq_rgb_ind_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_eq_hsv),   "clicked", G_CALLBACK (eq_hsv_clicked),     NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_eq_yuv),   "clicked", G_CALLBACK (eq_yuv_clicked),     NULL);
  

//For Match
  box_match = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_grid_attach (GTK_GRID (grid_a2), box_match, 2, 4, 1, 2);

  rbtn_mat_rgb_a = gtk_radio_button_new_with_label (NULL,"RGB-ALL");
  rbtn_mat_rgb_i = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_mat_rgb_a), "RGB-IND");
  rbtn_mat_hsv   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_mat_rgb_a), "HSV");
  rbtn_mat_s_v   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_mat_rgb_a), "HSV-V");
  rbtn_mat_yuv   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_mat_rgb_a), "YUV");
  rbtn_mat_s_y   = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_mat_rgb_a), "YUV-Y");
  
  gtk_box_pack_start (GTK_BOX (box_match), rbtn_mat_rgb_a,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_match), rbtn_mat_rgb_i,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_match), rbtn_mat_hsv,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_match), rbtn_mat_s_v,  1,0,0);
  gtk_box_pack_start (GTK_BOX (box_match), rbtn_mat_yuv,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_match), rbtn_mat_s_y,  1,0,0);
  
  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_mat_rgb_a), "clicked", G_CALLBACK (match_rgb_tog_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_mat_rgb_i), "clicked", G_CALLBACK (match_rgb_ind_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_mat_hsv),   "clicked", G_CALLBACK (match_hsv_clicked),     NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_mat_s_v),   "clicked", G_CALLBACK (match_v_clicked),       NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_mat_yuv),   "clicked", G_CALLBACK (match_yuv_clicked),     NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_mat_s_y),   "clicked", G_CALLBACK (match_y_clicked),       NULL);


/* -------------------------------------------------*
 *                      Buttons                     *
 * -------------------------------------------------*/
  
// eq button
  button_eq = gtk_button_new_with_label ("直方圖等化");
  g_signal_connect (button_eq, "clicked", G_CALLBACK (hist_equalize), NULL);
  /* Place in (0, 0), fill 1 cell horizontally and vertically (ie no spanning)*/
  gtk_grid_attach (GTK_GRID (grid_a2), button_eq, 3, 1, 1, 2);

// match button
  button_match = gtk_button_new_with_label ("直方圖匹配");
  g_signal_connect (button_match, "clicked", G_CALLBACK (hist_match), NULL);
  gtk_grid_attach (GTK_GRID (grid_a2), button_match, 3, 4, 1, 2);


/* -------------------------------------------------*
 *                  Entry & Label                   *
 * -------------------------------------------------*/
  ref_widg.label = gtk_label_new ("參考圖像：");
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.label, 0, 3, 1, 1);
  entry_ref = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (entry_ref), 50);
  g_signal_connect (entry_ref, "activate",
        G_CALLBACK (ref_enter_callback),
        entry_ref);
  gtk_grid_attach (GTK_GRID (grid_a2), entry_ref, 1, 3, 1, 1);


/* -------------------------------------------------*
 *                    Init Images                   *
 * -------------------------------------------------*/

// original image
  orig_widg.a2_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), orig_widg.a2_img, 0, 1, 1, 2);

  orig_widg.pdf_img = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), orig_widg.pdf_img, 1, 1, 1, 1);
  
  orig_widg.cdf_img = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), orig_widg.cdf_img, 1, 2, 1, 1);

  
// reference image
  ref_widg.a2_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.a2_img, 0, 4, 1, 2);

  ref_widg.pdf_img =gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.pdf_img, 1, 4, 1, 1);

  ref_widg.cdf_img =gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.cdf_img, 1, 5, 1, 1);


// hist_equalize output image
  eq_widg.a2_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), eq_widg.a2_img, 4, 1, 1, 2);
  
  eq_widg.pdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), eq_widg.pdf_img, 5, 1, 1, 1);
  
  eq_widg.cdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), eq_widg.cdf_img, 5, 2, 1, 1);

// histmatch output image
  match_widg.a2_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), match_widg.a2_img, 4, 4, 1, 2);
  
  match_widg.pdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), match_widg.pdf_img, 5, 4, 1, 1);
  
  match_widg.cdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), match_widg.cdf_img, 5, 5, 1, 1);



/******************************************************************
 ******************************************************************
 **                                                              **
 **                                                              **
 **                         assignment 3                         **
 **                                                              **
 **                                                              **
 ******************************************************************
 ******************************************************************/

/* -------------------------------------------------*
 *                  Radio Buttons                   *
 * -------------------------------------------------*/
//For Smooth
  box_smooth = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  gtk_grid_attach (GTK_GRID (grid_a3), box_smooth, 2, 1, 1, 1);
  
  rbtn_smt_blur = gtk_radio_button_new_with_label (NULL,"Blur");
  rbtn_smt_box  = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_smt_blur), "Box");
  rbtn_smt_gaus = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_smt_blur), "Gaussian");
  rbtn_smt_medi = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_smt_blur), "Median");
  rbtn_smt_bila = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_smt_blur), "Bilateral");

  gtk_box_pack_start (GTK_BOX (box_smooth), rbtn_smt_blur,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_smooth), rbtn_smt_box,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_smooth), rbtn_smt_gaus,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_smooth), rbtn_smt_medi,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_smooth), rbtn_smt_bila,1,0,0);

  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_smt_blur), "clicked", G_CALLBACK (sm_blur_clicked),      NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_smt_box),  "clicked", G_CALLBACK (sm_box_clicked),       NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_smt_gaus), "clicked", G_CALLBACK (sm_gaussian_clicked),  NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_smt_medi), "clicked", G_CALLBACK (sm_median_clicked),    NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_smt_bila), "clicked", G_CALLBACK (sm_bilateral_clicked), NULL);

//For EdgeDetect
  box_edge = gtk_box_new (GTK_ORIENTATION_VERTICAL, 7);
  gtk_grid_attach (GTK_GRID (grid_a3), box_edge, 3, 1, 1, 1);
  
  rbtn_edg_sob = gtk_radio_button_new_with_label (NULL,"Sobel");
  rbtn_edg_can = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_edg_sob), "Canny");
  rbtn_edg_sch = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_edg_sob), "Scharr");
  rbtn_edg_lap = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_edg_sob), "Laplace");
  rbtn_edg_rob = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_edg_sob), "Roberts");
  rbtn_edg_pre = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_edg_sob), "Prewitt");
  rbtn_edg_kir = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_edg_sob), "Kirsch");

  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_sob,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_can,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_sch,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_lap,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_rob,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_pre,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_edge), rbtn_edg_kir,1,0,0);

  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_sob), "clicked", G_CALLBACK (edge_sobel_clicked),   NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_can), "clicked", G_CALLBACK (edge_canny_clicked),   NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_sch), "clicked", G_CALLBACK (edge_scharr_clicked),  NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_lap), "clicked", G_CALLBACK (edge_laplace_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_rob), "clicked", G_CALLBACK (edge_roberts_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_pre), "clicked", G_CALLBACK (edge_prewitt_clicked), NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_edg_kir), "clicked", G_CALLBACK (edge_kirsch_clicked),  NULL);

//For image sharpening
  box_sharpen = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_grid_attach (GTK_GRID (grid_a3), box_sharpen, 6, 1, 1, 1);
  
  rbtn_shp_lap1 = gtk_radio_button_new_with_label (NULL,"Laplace type1");
  rbtn_shp_lap2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_shp_lap1), "Laplace type2");
  rbtn_shp_log  = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_shp_lap1), "Second order LoG");
  rbtn_shp_uns  = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (rbtn_shp_lap1), "Unsharp mask");

  gtk_box_pack_start (GTK_BOX (box_sharpen), rbtn_shp_lap1,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_sharpen), rbtn_shp_lap2,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_sharpen), rbtn_shp_log,1,0,0);
  gtk_box_pack_start (GTK_BOX (box_sharpen), rbtn_shp_uns,1,0,0);

  /*Connect the signal handlers (aka Callback functions) to the buttons*/
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_shp_lap1), "clicked", G_CALLBACK (shp_lap1_clicked),    NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_shp_lap2), "clicked", G_CALLBACK (shp_lap2_clicked),    NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_shp_log),  "clicked", G_CALLBACK (shp_seclog_clicked),  NULL);
  g_signal_connect (GTK_TOGGLE_BUTTON (rbtn_shp_uns),  "clicked", G_CALLBACK (shp_unsharp_clicked), NULL);


/* -------------------------------------------------*
 *                      Labels                      *
 * -------------------------------------------------*/
  output_widg.label=gtk_label_new ("輸出圖像：");
  gtk_grid_attach (GTK_GRID (grid_a3), output_widg.label, 0, 3, 1, 1);
  lb_ksize=gtk_label_new ("Kernel大小（%）");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_ksize, 1, 0, 1, 1);
  lb_smooth=gtk_label_new ("平滑處理方式");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_smooth, 2, 0, 1, 1);
  lb_edge=gtk_label_new ("邊緣偵測方式");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_edge, 3, 0, 1, 1);
  lb_can_thrd1=gtk_label_new ("Canny thrd1");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_can_thrd1, 4, 0, 1, 1);
  lb_can_thrd2=gtk_label_new ("Canny thrd2");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_can_thrd2, 5, 0, 1, 1);
  lb_sharpen=gtk_label_new ("銳利化方式");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_sharpen, 6, 0, 1, 1);
  lb_landa=gtk_label_new ("Landa");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_landa, 7, 0, 1, 1);
  lb_gain_diff=gtk_label_new ("差值增益");
  gtk_grid_attach (GTK_GRID (grid_a3), lb_gain_diff, 1, 3, 1, 1);
  diff_widg.label=gtk_label_new ("前後變化");
  gtk_grid_attach (GTK_GRID (grid_a3), diff_widg.label, 2, 3, 1, 1);


/* -------------------------------------------------*
 *                      Scales                      *
 * -------------------------------------------------*/
  adj_ksize=gtk_adjustment_new(0,0,100,1,1,0);
  scale_ksize=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_ksize));
  g_signal_connect (scale_ksize, "value-changed", G_CALLBACK (ksize_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a3), scale_ksize, 1, 1, 1, 2);

  adj_canny_thrd1=gtk_adjustment_new(cv2ip.canny_threshold1,0,500,1,1,0);
  scale_canny_thrd1=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_canny_thrd1));
  g_signal_connect (scale_canny_thrd1, "value-changed", G_CALLBACK (canny_thrd1_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a3), scale_canny_thrd1, 4, 1, 1, 1);

  adj_canny_thrd2=gtk_adjustment_new(cv2ip.canny_threshold2,0,500,1,1,0);
  scale_canny_thrd2=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_canny_thrd2));
  g_signal_connect (scale_canny_thrd2, "value-changed", G_CALLBACK (canny_thrd2_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a3), scale_canny_thrd2, 5, 1, 1, 1);

  adj_landa=gtk_adjustment_new(cv2ip.landa,0,5,1,1,0);
  scale_landa=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_landa));
  g_signal_connect (scale_landa, "value-changed", G_CALLBACK (landa_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a3), scale_landa, 7, 1, 1, 1);

  adj_diff_gain=gtk_adjustment_new(diff_gain,0,10,1,1,0);
  scale_diff_gain=gtk_scale_new(GTK_ORIENTATION_VERTICAL,GTK_ADJUSTMENT(adj_diff_gain));
  g_signal_connect (scale_diff_gain, "value-changed", G_CALLBACK (diff_gain_changed), NULL);
  gtk_grid_attach (GTK_GRID (grid_a3), scale_diff_gain, 1, 4, 1, 1);


/* -------------------------------------------------*
 *                      Buttons                     *
 * -------------------------------------------------*/
  // smooth button
  button_sm = gtk_button_new_with_label ("平滑處理");
  g_signal_connect (button_sm, "clicked", G_CALLBACK (img_smooth), NULL);
  /* Place in (0, 0), fill 1 cell horizontally and vertically (ie no spanning)*/
  gtk_grid_attach (GTK_GRID (grid_a3), button_sm, 2, 2, 1, 1);

  // edge detect button
  button_edge = gtk_button_new_with_label ("邊緣偵測");
  g_signal_connect (button_edge, "clicked", G_CALLBACK (edge_detect), NULL);
  /* Place in (0, 0), fill 1 cell horizontally and vertically (ie no spanning)*/
  gtk_grid_attach (GTK_GRID (grid_a3), button_edge, 3, 2, 3, 1);

  // sharpening button
  button_sharpen = gtk_button_new_with_label ("銳利化");
  g_signal_connect (button_sharpen, "clicked", G_CALLBACK (img_sharpen), NULL);
  /* Place in (0, 0), fill 1 cell horizontally and vertically (ie no spanning)*/
  gtk_grid_attach (GTK_GRID (grid_a3), button_sharpen, 6, 2, 2, 1);


/* -------------------------------------------------*
 *                    Init Images                   *
 * -------------------------------------------------*/

// original image
  orig_widg.a3_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a3), orig_widg.a3_img, 0, 0, 1, 3);

// output image
  output_widg.a3_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(output_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a3), output_widg.a3_img, 0, 4, 1, 1);

// diff image
  diff_widg.a3_img  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(diff_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a3), diff_widg.a3_img, 2, 4, 6, 1);



/* -------------------------------------------------*
 *                       show                       *
 * -------------------------------------------------*/
  gtk_widget_show_all (window);
  gtk_main ();

  return 0;
}


/* -------------------------------------------------*
 *                     functions                    *
 * -------------------------------------------------*/
static void orig_init(char *filename)
{
  try
  {
    orig_widg.pic_cvImg=cv2ip.ImRead(comb_char(src_dir,filename));

    orig_size_min=min(orig_widg.pic_cvImg.rows,orig_widg.pic_cvImg.cols);
    cv2ip.kernel_size=((int)gtk_range_get_value (GTK_RANGE(scale_ksize))) * orig_size_min / 3000 * 2 + 3;
    
    orig_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,filename),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.a1_img),orig_widg.pixbuf);
    gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.a2_img),orig_widg.pixbuf);
    gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.a3_img),orig_widg.pixbuf);
    
    cv2ip.ShowCDF(orig_widg.pic_cvImg,orig_widg.pdf_cvImg,orig_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(pdf_dir,filename),orig_widg.pdf_cvImg);
    cv2ip.ImWrite(comb_char(cdf_dir,filename),orig_widg.cdf_cvImg);

    orig_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,filename),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.pdf_img),orig_widg.pdf_pix);
    orig_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,filename),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.cdf_img),orig_widg.cdf_pix);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "orig init failed" << endl;
  }
}

static void ref_init()
{
  try
  {
    ref_widg.pic_cvImg=cv2ip.ImRead(comb_char(src_dir,ref_name));
    ref_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,ref_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.a2_img),ref_widg.pixbuf);

    cv2ip.ShowCDF(ref_widg.pic_cvImg,ref_widg.pdf_cvImg,ref_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(pdf_dir,ref_name),ref_widg.pdf_cvImg);
    cv2ip.ImWrite(comb_char(cdf_dir,ref_name),ref_widg.cdf_cvImg);

    ref_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,ref_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.pdf_img),ref_widg.pdf_pix);
    ref_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,ref_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.cdf_img),ref_widg.cdf_pix);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "ref init failed" << endl;
  }
  
}

static void fore_init()
{
  try
  {
    fore_widg.pic_cvImg = cv2ip.ImRead(comb_char(src_dir,fore_name));
    fore_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,fore_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(fore_widg.a1_img),fore_widg.pixbuf);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "fore init failed" << endl;
  }
  
}

static void show_diff()
{
  try
  {
    cv2ip.ShowDiff(orig_widg.pic_cvImg,output_widg.pic_cvImg,diff_gain,comb_char(diff_dir,diff_name));
    diff_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(diff_dir,diff_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(diff_widg.a3_img),diff_widg.pixbuf);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "show diff failed" << endl;
  }
  
}

static void blend_img()
{
  try
  {
    double w_ratio=(double)(fore_widg.pic_cvImg.cols)/max(fore_widg.pic_cvImg.cols,fore_widg.pic_cvImg.rows);
    double h_ratio=(double)(fore_widg.pic_cvImg.rows)/max(fore_widg.pic_cvImg.cols,fore_widg.pic_cvImg.rows);

    int size_w=min(orig_widg.pic_cvImg.cols,orig_widg.pic_cvImg.rows)*width*w_ratio/100;
    int size_h=min(orig_widg.pic_cvImg.cols,orig_widg.pic_cvImg.rows)*height*h_ratio/100;

    int pos_x=(orig_widg.pic_cvImg.cols-size_w)*x/100;
    int pos_y=(orig_widg.pic_cvImg.rows-size_h)*y/100;

    cv2ip.BlendImage( blend_widg.pic_cvImg,fore_widg.pic_cvImg,orig_widg.pic_cvImg,size_w,size_h,pos_x,pos_y);
    cv2ip.ImWrite(comb_char(img_dir,blend_name),blend_widg.pic_cvImg);
    cv2ip.ImWrite(comb_char(src_dir,output_name),blend_widg.pic_cvImg);
    blend_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,blend_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(blend_widg.a1_img),blend_widg.pixbuf);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "blend img failed" << endl;
  }
  
}

/* -------------------------------------------------*
 *                Callback functions                *
 * -------------------------------------------------*/
static void load_in (GtkWidget *widget, gpointer data)
{
  orig_init(output_name);
}

/*Signal handler for the "toggled" signal of the RadioButton*/
static void eq_rgb_tog_clicked (GtkWidget *button, gpointer   data)
{
  eq_type=EQ_RGB_TOG;
}
static void eq_rgb_ind_clicked (GtkWidget *button, gpointer   data)
{
  eq_type=EQ_RGB_IND;
}
static void eq_hsv_clicked (GtkWidget *button, gpointer   data)
{
  eq_type=EQ_HSV;
}
static void eq_yuv_clicked (GtkWidget *button, gpointer   data)
{
  eq_type=EQ_YUV;
}


/*Signal handler for the "toggled" signal of the RadioButton*/
static void match_rgb_tog_clicked (GtkWidget *button, gpointer   data)
{
  match_type=MAT_RGB_TOG;
}
static void match_rgb_ind_clicked (GtkWidget *button, gpointer   data)
{
  match_type=MAT_RGB_IND;
}
static void match_hsv_clicked (GtkWidget *button, gpointer   data)
{
  match_type=MAT_HSV;
}
static void match_v_clicked (GtkWidget *button, gpointer   data)
{
  match_type=MAT_V;
}
static void match_yuv_clicked (GtkWidget *button, gpointer   data)
{
  match_type=MAT_YUV;
}
static void match_y_clicked (GtkWidget *button, gpointer   data)
{
  match_type=MAT_Y;
}



static void sm_blur_clicked (GtkWidget *button, gpointer   data)
{
  smooth_type=cv2ip.BLUR;
}
static void sm_box_clicked (GtkWidget *button, gpointer   data)
{
  smooth_type=cv2ip.BOX;
}
static void sm_gaussian_clicked (GtkWidget *button, gpointer   data)
{
  smooth_type=cv2ip.GAUSSIAN;
}
static void sm_median_clicked (GtkWidget *button, gpointer   data)
{
  smooth_type=cv2ip.MEDIAN;
}
static void sm_bilateral_clicked (GtkWidget *button, gpointer   data)
{
  smooth_type=cv2ip.BILATERAL;
}



/*Signal handler for the "clicked" signal of the EdgeDetect RadioButton*/
static void edge_sobel_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.SOBEL;
}
static void edge_canny_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.CANNY;
}
static void edge_scharr_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.SCHARR;
}
static void edge_laplace_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.LAPLACE;
}
static void edge_roberts_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.ROBERTS;
}
static void edge_prewitt_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.PREWITT;
}
static void edge_kirsch_clicked (GtkWidget *button, gpointer   data)
{
  edge_detect_type=cv2ip.KIRSCH;
}



/*Signal handler for the "clicked" signal of the sharpen RadioButton*/
static void shp_lap1_clicked (GtkWidget *button, gpointer   data)
{
  sharpen_type=cv2ip.LAPLACE_TYPE1;
}
static void shp_lap2_clicked (GtkWidget *button, gpointer   data)
{
  sharpen_type=cv2ip.LAPLACE_TYPE2;
}
static void shp_seclog_clicked (GtkWidget *button, gpointer   data)
{
  sharpen_type=cv2ip.SECOND_ORDER_LOG;
}
static void shp_unsharp_clicked (GtkWidget *button, gpointer   data)
{
  sharpen_type=cv2ip.UNSHARP_MASK;
}


static void hist_equalize (GtkWidget *widget, gpointer data)
{
  // try
  // {
    switch (eq_type)
    {
    case EQ_RGB_TOG:
      cv2ip.AllChEqualize(eq_widg.pic_cvImg,orig_widg.pic_cvImg);
      break;
    case EQ_RGB_IND:
      cv2ip.ColorEqualize(eq_widg.pic_cvImg,orig_widg.pic_cvImg);
      break;
    case EQ_HSV:
      cvtColor(orig_widg.pic_cvImg,orig_widg.hsv_cvImg,COLOR_BGR2HSV);
      cv2ip.ColorEqualize(eq_widg.pic_cvImg,orig_widg.hsv_cvImg,cv2ip.USE_HSV);
      cvtColor(eq_widg.pic_cvImg,eq_widg.pic_cvImg,COLOR_HSV2BGR);
      break;
    case EQ_YUV:
      cvtColor(orig_widg.pic_cvImg,orig_widg.yuv_cvImg,COLOR_BGR2YUV);
      cv2ip.ColorEqualize(eq_widg.pic_cvImg,orig_widg.yuv_cvImg,cv2ip.USE_YUV);
      cvtColor(eq_widg.pic_cvImg,eq_widg.pic_cvImg,COLOR_YUV2BGR);
      break;
    default:
      break;
    }
    cv2ip.ShowCDF(eq_widg.pic_cvImg,eq_widg.pdf_cvImg,eq_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(img_dir,eq_name),eq_widg.pic_cvImg);
    cv2ip.ImWrite(comb_char(pdf_dir,eq_name),eq_widg.pdf_cvImg);
    cv2ip.ImWrite(comb_char(cdf_dir,eq_name),eq_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(src_dir,output_name),eq_widg.pic_cvImg);
    eq_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,eq_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(eq_widg.a2_img),eq_widg.pixbuf);
    eq_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,eq_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(eq_widg.pdf_img),eq_widg.pdf_pix);
    eq_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,eq_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(eq_widg.cdf_img),eq_widg.cdf_pix);
  // }
  // catch(const std::exception& e)
  // {
  //   std::cerr << e.what() << '\n';
  //   std::cout << "hist equalize failed" << endl;
  // }
  
}

static void hist_match (GtkWidget *widget, gpointer data)
{
  // try
  // {
    switch (match_type)
    {
    case MAT_RGB_TOG:
      cv2ip.HistMatchAll(match_widg.pic_cvImg,orig_widg.pic_cvImg,ref_widg.pic_cvImg);
      break;
    case MAT_RGB_IND:
      cv2ip.HistMatching(match_widg.pic_cvImg,orig_widg.pic_cvImg,ref_widg.pic_cvImg);
      break;
    case MAT_HSV:
      cvtColor(orig_widg.pic_cvImg,orig_widg.hsv_cvImg,COLOR_BGR2HSV);
      cvtColor(ref_widg.pic_cvImg,ref_widg.hsv_cvImg,COLOR_BGR2HSV);
      cv2ip.HistMatching(match_widg.pic_cvImg,orig_widg.hsv_cvImg,ref_widg.hsv_cvImg,cv2ip.USE_RGB);
      cvtColor(match_widg.pic_cvImg,match_widg.pic_cvImg,COLOR_HSV2BGR);
      break;
    case MAT_V:
      cvtColor(orig_widg.pic_cvImg,orig_widg.hsv_cvImg,COLOR_BGR2HSV);
      cvtColor(ref_widg.pic_cvImg,ref_widg.hsv_cvImg,COLOR_BGR2HSV);
      cv2ip.HistMatching(match_widg.pic_cvImg,orig_widg.hsv_cvImg,ref_widg.hsv_cvImg,cv2ip.USE_HSV);
      cvtColor(match_widg.pic_cvImg,match_widg.pic_cvImg,COLOR_HSV2BGR);
      break;
    case MAT_YUV:
      cvtColor(orig_widg.pic_cvImg,orig_widg.yuv_cvImg,COLOR_BGR2YUV);
      cvtColor(ref_widg.pic_cvImg,ref_widg.yuv_cvImg,COLOR_BGR2YUV);
      cv2ip.HistMatching(match_widg.pic_cvImg,orig_widg.yuv_cvImg,ref_widg.yuv_cvImg,cv2ip.USE_RGB);
      cvtColor(match_widg.pic_cvImg,match_widg.pic_cvImg,COLOR_YUV2BGR);
      break;
    case MAT_Y:
      cvtColor(orig_widg.pic_cvImg,orig_widg.yuv_cvImg,COLOR_BGR2YUV);
      cvtColor(ref_widg.pic_cvImg,ref_widg.yuv_cvImg,COLOR_BGR2YUV);
      cv2ip.HistMatching(match_widg.pic_cvImg,orig_widg.yuv_cvImg,ref_widg.yuv_cvImg,cv2ip.USE_YUV);
      cvtColor(match_widg.pic_cvImg,match_widg.pic_cvImg,COLOR_YUV2BGR);
      break;
    
    default:
      break;
    }
    
    cv2ip.ShowCDF(match_widg.pic_cvImg,match_widg.pdf_cvImg,match_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(img_dir,match_name),match_widg.pic_cvImg);
    cv2ip.ImWrite(comb_char(pdf_dir,match_name),match_widg.pdf_cvImg);
    cv2ip.ImWrite(comb_char(cdf_dir,match_name),match_widg.cdf_cvImg);
    cv2ip.ImWrite(comb_char(src_dir,output_name),match_widg.pic_cvImg);
    match_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,match_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(match_widg.a2_img),match_widg.pixbuf);
    match_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,match_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(match_widg.pdf_img),match_widg.pdf_pix);
    match_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,match_name),200,200,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(match_widg.cdf_img),match_widg.cdf_pix);
  // }
  // catch(const std::exception& e)
  // {
  //   std::cerr << e.what() << '\n';
  //   std::cout << "hist match failed" << endl;
  // }
  
}

static void img_smooth (GtkWidget *widget, gpointer data)
{
  try
  {
    cv2ip.Smooth2D(output_widg.pic_cvImg,orig_widg.pic_cvImg,cv2ip.kernel_size,smooth_type);
    cv2ip.ImWrite(comb_char(img_dir,smooth_name),output_widg.pic_cvImg);
    cv2ip.ImWrite(comb_char(src_dir,output_name),output_widg.pic_cvImg);
    output_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,smooth_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(output_widg.a3_img),output_widg.pixbuf);
    show_diff();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "img smooth failed" << endl;
  }
  
}

static void edge_detect (GtkWidget *widget, gpointer data)
{
  try
  {
    cv2ip.EdgeDetect(output_widg.pic_cvImg,orig_widg.pic_cvImg,edge_detect_type);
    cv2ip.ImWrite(comb_char(img_dir,edge_name),output_widg.pic_cvImg);
    cv2ip.ImWrite(comb_char(src_dir,output_name),output_widg.pic_cvImg);
    output_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,edge_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(output_widg.a3_img),output_widg.pixbuf);
    show_diff();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "edge detect failed" << endl;
  }
  
}


static void img_sharpen (GtkWidget *widget, gpointer data)
{
  try
  {
    cv2ip.ImSharpening(output_widg.pic_cvImg,orig_widg.pic_cvImg,sharpen_type,smooth_type);
    cv2ip.ImWrite(comb_char(img_dir,sharpen_name),output_widg.pic_cvImg);
    cv2ip.ImWrite(comb_char(src_dir,output_name),output_widg.pic_cvImg);
    output_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(img_dir,sharpen_name),800,400,err);
    gtk_image_set_from_pixbuf(GTK_IMAGE(output_widg.a3_img),output_widg.pixbuf);
    show_diff();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cout << "img sharpen failed" << endl;
  }
  
}

static void ksize_changed (GtkWidget *widget, gpointer data)
{
  cv2ip.kernel_size=((int)gtk_range_get_value (GTK_RANGE(scale_ksize))) * orig_size_min / 3000 * 2 + 3;
}

static void canny_thrd1_changed (GtkWidget *widget, gpointer data)
{
  cv2ip.canny_threshold1=gtk_range_get_value (GTK_RANGE(scale_canny_thrd1));
}
static void canny_thrd2_changed (GtkWidget *widget, gpointer data)
{
  cv2ip.canny_threshold2=gtk_range_get_value (GTK_RANGE(scale_canny_thrd2));
}

static void landa_changed (GtkWidget *widget, gpointer data)
{
  cv2ip.landa=gtk_range_get_value (GTK_RANGE(scale_landa));
}

static void diff_gain_changed (GtkWidget *widget, gpointer data)
{
  diff_gain=gtk_range_get_value (GTK_RANGE(scale_diff_gain));
  show_diff();
}

static void orig_enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(orig_name,entry_text);
  orig_init(orig_name);
}
static void ref_enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(ref_name,entry_text);
  ref_init();
}

static void width_changed (GtkWidget *widget, gpointer data)
{
  width=gtk_range_get_value (GTK_RANGE(scale_width));
  blend_img();
}

static void height_changed (GtkWidget *widget, gpointer data)
{
  height=gtk_range_get_value (GTK_RANGE(scale_height));
  blend_img();
}

static void x_changed (GtkWidget *widget, gpointer data)
{
  x=gtk_range_get_value (GTK_RANGE(scale_x));
  blend_img();
}

static void y_changed (GtkWidget *widget, gpointer data)
{
  y=gtk_range_get_value (GTK_RANGE(scale_y));
  blend_img();
}

static void blend_clicked (GtkWidget *widget, gpointer data)
{
  blend_img();
}

static void fore_enter_callback( GtkWidget *widget, GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(fore_name,entry_text);
  fore_init();
}

/* -------------------------------------------------*
 *                    custom func                   *
 * -------------------------------------------------*/
char* comb_char(char* ch1, char* ch2)
{
  char* ret_ch=new char[strlen(ch1)+strlen(ch2)+1];// how to delete it after return?
  strcpy(ret_ch,ch1);
  strcat(ret_ch,ch2);
  return ret_ch;
}
