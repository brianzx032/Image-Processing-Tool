#include "mainWindow.h"

/******************************************************************
 ******************************************************************
 **                                                              **
 **                                                              **
 **                     Not Yet Finished                         **
 **                                                              **
 **                                                              **
 ******************************************************************
 ******************************************************************/

/* -------------------------------------------------*
 *            Constructor & Deconstructor           *
 * -------------------------------------------------*/
mainWindow::mainWindow(void)
{
    std::cout << "mainWindow is being created" << std::endl;
}
mainWindow::~mainWindow(void)
{
    std::cout << "mainWindow is being deleted" << std::endl;
}
void mainWindow::windowInit(int   argc, char *argv[])
{
    // filename
    char src_dir[]="./img/src/";
    char img_dir[]="./img/output/";
    char pdf_dir[]="./img/pdf/pdf_";
    char cdf_dir[]="./img/cdf/cdf_";
    char orig_name[]="";
    char ref_name[]="";
    char eq_name[]="eq.jpg";
    char match_name[]="match.jpg";

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
  gtk_window_set_title (GTK_WINDOW (window), "Grid");
  gtk_widget_set_size_request (window, 100,100);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 20);


/* -------------------------------------------------*
 *                        grid                      *
 * -------------------------------------------------*/
  /* Here we construct the container that is going pack our pages */
  grid_stack = gtk_grid_new ();
  grid_a1 = gtk_grid_new ();
  grid_a2 = gtk_grid_new ();
  grid_a3 = gtk_grid_new ();
  

/* -------------------------------------------------*
 *                        stack                     *
 * -------------------------------------------------*/
  stack_pages=gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(stack_pages),GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
  gtk_stack_set_transition_duration(GTK_STACK(stack_pages),500);
  
  gtk_stack_add_titled (GTK_STACK(stack_pages), GTK_WIDGET(grid_a1), "a1_nm", "a1_ttl");
  gtk_stack_add_titled (GTK_STACK(stack_pages), GTK_WIDGET(grid_a2), "a2_nm", "a2_ttl");
  gtk_stack_add_titled (GTK_STACK(stack_pages), GTK_WIDGET(grid_a3), "a3_nm", "a3_ttl");

  stack_sw=gtk_stack_switcher_new();
  gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_sw),GTK_STACK(stack_pages));

  gtk_container_add (GTK_CONTAINER (window), grid_stack);
  gtk_grid_attach (GTK_GRID (grid_stack), stack_pages, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid_stack), stack_sw, 0, 0, 1, 1);


/* -------------------------------------------------*
 *                      Buttons                     *
 * -------------------------------------------------*/
// quit button
  button_quit = gtk_button_new_with_label ("Quit");
  g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
  gtk_grid_attach (GTK_GRID (grid_stack), button_quit, 0, 2, 1, 1);


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
  boxEq = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_grid_attach (GTK_GRID (grid_a2), boxEq, 2, 1, 1, 2);
  
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
  gtk_grid_attach (GTK_GRID (grid_a2), boxMatch, 2, 4, 1, 2);

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
  orig_label = gtk_label_new ("原圖檔：");
  gtk_grid_attach (GTK_GRID (grid_a2), orig_label, 0, 0, 1, 1);
  orig_entry = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (orig_entry), 50);
  g_signal_connect (orig_entry, "activate",
        G_CALLBACK (orig_enter_callback),
        orig_entry);
  gtk_grid_attach (GTK_GRID (grid_a2), orig_entry, 1, 0, 5, 1);

  ref_label = gtk_label_new ("參考圖檔：");
  gtk_grid_attach (GTK_GRID (grid_a2), ref_label, 0, 3, 1, 1);
  ref_entry = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (ref_entry), 50);
  g_signal_connect (ref_entry, "activate",
        G_CALLBACK (ref_enter_callback),
        ref_entry);
  gtk_grid_attach (GTK_GRID (grid_a2), ref_entry, 1, 3, 5, 1);


/* -------------------------------------------------*
 *                    Init Images                   *
 * -------------------------------------------------*/

// original image
  orig_widg.image  = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), orig_widg.image, 0, 1, 1, 2);

  orig_widg.pdf_img = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), orig_widg.pdf_img, 1, 1, 1, 1);
  
  orig_widg.cdf_img = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(orig_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), orig_widg.cdf_img, 1, 2, 1, 1);
  
// reference image
  ref_widg.image=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.image, 0, 4, 1, 2);

  ref_widg.pdf_img =gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.pdf_img, 1, 4, 1, 1);

  ref_widg.cdf_img =gtk_image_new_from_pixbuf(gdk_pixbuf_copy(ref_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), ref_widg.cdf_img, 1, 5, 1, 1);

// hist_equalize output image
  eq_widg.image=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), eq_widg.image, 4, 1, 1, 2);
  
  eq_widg.pdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), eq_widg.pdf_img, 5, 1, 1, 1);
  
  eq_widg.cdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(eq_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), eq_widg.cdf_img, 5, 2, 1, 1);

// histmatch output image
  match_widg.image=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.pixbuf));
  gtk_grid_attach (GTK_GRID (grid_a2), match_widg.image, 4, 4, 1, 2);
  
  match_widg.pdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.pdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), match_widg.pdf_img, 5, 4, 1, 1);
  
  match_widg.cdf_img=gtk_image_new_from_pixbuf(gdk_pixbuf_copy(match_widg.cdf_pix));
  gtk_grid_attach (GTK_GRID (grid_a2), match_widg.cdf_img, 5, 5, 1, 1);


/* -------------------------------------------------*
 *                       show                       *
 * -------------------------------------------------*/
  gtk_widget_show_all (window);
  gtk_main ();
}
/* -------------------------------------------------*
 *                Callback functions                *
 * -------------------------------------------------*/
/*Signal handler for the "toggled" signal of the RadioButton*/
 void mainWindow::eq_rgb_tog_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_RGB_TOG;
}
 void mainWindow::eq_rgb_ind_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_RGB_IND;
}
 void mainWindow::eq_hsv_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_HSV;
}
 void mainWindow::eq_yuv_clicked (GtkWidget *button, gpointer   user_data)
{
  eq_type=EQ_YUV;
}


/*Signal handler for the "toggled" signal of the RadioButton*/
 void mainWindow::match_rgb_tog_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_RGB_TOG;
}
 void mainWindow::match_rgb_ind_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_RGB_IND;
}
 void mainWindow::match_hsv_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_HSV;
}
 void mainWindow::match_v_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_V;
}
 void mainWindow::match_yuv_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_YUV;
}
 void mainWindow::match_y_clicked (GtkWidget *button, gpointer   user_data)
{
  match_type=MAT_Y;
}


 void mainWindow::hist_equalize (GtkWidget *widget, gpointer data)
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

 void mainWindow::hist_match (GtkWidget *widget, gpointer data)
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

 void mainWindow::orig_enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(orig_name,entry_text);
  img_orig=cv2ip.ImRead(comb_char(src_dir,orig_name));
  orig_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,orig_name),800,400,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.image),orig_widg.pixbuf);
  cv2ip.ShowCDF(img_orig,orig_widg.pdf_cvImg,orig_widg.cdf_cvImg);
  cv2ip.ImWrite(comb_char(pdf_dir,orig_name),orig_widg.pdf_cvImg);
  cv2ip.ImWrite(comb_char(cdf_dir,orig_name),orig_widg.cdf_cvImg);
  orig_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,orig_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.pdf_img),orig_widg.pdf_pix);
  orig_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,orig_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(orig_widg.cdf_img),orig_widg.cdf_pix);
  
}
 void mainWindow::ref_enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  strcpy(ref_name,entry_text);
  img_ref=cv2ip.ImRead(comb_char(src_dir,ref_name));
  ref_widg.pixbuf = gdk_pixbuf_new_from_file_at_size(comb_char(src_dir,ref_name),800,400,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.image),ref_widg.pixbuf);
  cv2ip.ShowCDF(img_ref,ref_widg.pdf_cvImg,ref_widg.cdf_cvImg);
  cv2ip.ImWrite(comb_char(pdf_dir,ref_name),ref_widg.pdf_cvImg);
  cv2ip.ImWrite(comb_char(cdf_dir,ref_name),ref_widg.cdf_cvImg);
  ref_widg.pdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(pdf_dir,ref_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.pdf_img),ref_widg.pdf_pix);
  ref_widg.cdf_pix = gdk_pixbuf_new_from_file_at_size(comb_char(cdf_dir,ref_name),200,200,err);
  gtk_image_set_from_pixbuf(GTK_IMAGE(ref_widg.cdf_img),ref_widg.cdf_pix);
}
/* -------------------------------------------------*
 *                    custom func                   *
 * -------------------------------------------------*/
char* mainWindow::comb_char(char* ch1, char* ch2)
{
  char* ret_ch=new char[strlen(ch1)+strlen(ch2)+1];
  strcpy(ret_ch,ch1);
  strcat(ret_ch,ch2);
  return ret_ch;
}