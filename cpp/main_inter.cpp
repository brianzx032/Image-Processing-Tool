#include <gtk/gtk.h>
#include <iostream>
#include "src/cv2ImageProcessing.h"
using namespace cv;
using namespace std;

static void
print_hello (GtkWidget *widget,
     gpointer   data)
{
    cv2ImageProcessing cv2ip;
    CvImage pinksky,skymask,newimg;
    newimg=cv2ip.ImRead("sky.jpeg");

    cv2ip.ImShow("newimg", newimg); 
    cv::waitKey(0); 
    
    
    // CvImage sky=cv2ip.ImRead("flower.jpg");

    // // pinksky.create(pinksky.rows*3, pinksky.cols*3, CV_8U);
    // cv::resize(newimg,pinksky,newimg.size()*3);
    
    // // CvImage myroi2=cv2ip.ImRead("mcdonald.png");
    // CvImage myroi = pinksky(cv::Rect(25,30,sky.cols,sky.rows));
    // CvImage myroi2 = pinksky(cv::Rect(20,50,sky.cols,sky.rows));
    // // CvImage myroi2 = (cv::Rect(20,50,sky.cols,sky.rows));
    // // cout<<&pinksky<<endl<<&myroi<<endl;
    // skymask=sky(cv::Rect(0,0,sky.cols,sky.rows));
    // cv2ip.ImShow("ROI", myroi); 
    // waitKey(0); 
    // myroi2=cv2ip.EqualizeColorHist(myroi);

    // // myroi = cv::Mat::zeros(myroi.size(), myroi.type());
    // // sky.convertTo(myroi,myroi.type());
    // // myroi2.copyTo(myroi);
    // // myroi = sky.clone();
    // addWeighted(myroi, 0.2, sky, 0.8, 0.0, myroi);//dst = src1[I]*alpha+ src2[I]*beta + gamma；第一第四个参数就是各自权重，第5个参数就是公式中的偏执因子gamma。
    // cv2ip.ImShow("ROI", myroi); 
    // waitKey(0); 
    // cv2ip.ImWrite("pinksky_1.jpg",pinksky);
    // cv2ip.ImShow("pinksky", pinksky); 
    // waitKey(0); 
}

int
main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *image1;
  GtkWidget *image2;

  /* This is called in all GTK applications. Arguments are parsed
   * from the command line and are returned to the application.
   */
  gtk_init (&argc, &argv);

  /* create a new window, and set its title */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Grid");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 100);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();

  /* Pack the container in the window */
  gtk_container_add (GTK_CONTAINER (window), grid);

  button = gtk_button_new_with_label ("Button 1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label ("Button 2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

  button = gtk_button_new_with_label ("Quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 1);

  /* Now that we are done packing our widgets, we show them all
   * in one go, by calling gtk_widget_show_all() on the window.
   * This call recursively calls gtk_widget_show() on all widgets
   * that are contained in the window, directly or indirectly.
   */
  image1=gtk_image_new_from_file("flower.jpg");
  gtk_container_add(GTK_CONTAINER (grid), image1);
  image2=gtk_image_new_from_file("flower.jpg");
  gtk_container_add(GTK_CONTAINER (grid), image2);
  gtk_widget_show_all (window);

  /* All GTK applications must have a gtk_main(). Control ends here
   * and waits for an event to occur (like a key press or a mouse event),
   * until gtk_main_quit() is called.
   */
  gtk_main ();

  return 0;
}

