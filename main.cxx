#include "vtkPlaneSource.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkSmartPointer.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include <vtkTransform.h>


bool EqualPixels(int* p0, int *p1)
{
  bool res = true;
  for(int i = 0; i <3; i++)
  {
     if(p0[i] != p1[i])
     {
       res = false;
     }
  }
  return res;
}

int* GetPixelValue(vtkImageData * im, int x , int y)
{
  int * px = new int[3]; // We don't care about memory leaks in this example.
  for( int i = 0 ; i < 3; i++)
  {
    px[i]= int(im->GetScalarComponentAsFloat(x,y,0,i));
  }
  return px;
}

void PrintValue(vtkImageData * im, int x , int y)
{
  std::cout << "(x:" << x << " y:" << y << ") RGB: ";
  for( int i = 0 ; i < 3; i++)
  {
    std::cout << im->GetScalarComponentAsFloat(x, y, 0, i) << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout<<"Usage: "<<argv[0] << " output_image"<<std::endl;
    return 1;
  }
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
  // Turning Off-screen rendering to True removes the bug
  //window->SetOffScreenRendering( True ); 
  window->SetSize(640, 480);

  window->AddRenderer(renderer);
  vtkCamera* camera = renderer->GetActiveCamera();
  camera->SetPosition(0., 0., 0.);
  camera->SetFocalPoint(0., 0., 1.);  // Sets z-forward.
  camera->SetViewUp(0., -1, 0.);  // Sets y-down.
  // Changing the angle ever so slightly removes the bug: bug appears
  // with 43,44,45,46, but not with 30 or 47.
  camera->SetViewAngle(45);

  vtkSmartPointer<vtkMatrix4x4> vtk_mat = vtkSmartPointer<vtkMatrix4x4>::New();
  vtk_mat->SetElement(0, 0, 0);
  vtk_mat->SetElement(0, 1, -1.0f);
  vtk_mat->SetElement(0, 2, 0);
  vtk_mat->SetElement(0, 3, 0);
  vtk_mat->SetElement(1, 0, -1.0f);
  vtk_mat->SetElement(1, 1, 0);
  vtk_mat->SetElement(1, 2, 0);
  vtk_mat->SetElement(1, 3, 0);
  vtk_mat->SetElement(2, 0, 0);
  vtk_mat->SetElement(2, 1, 0);
  vtk_mat->SetElement(2, 2, -1.0f);
  vtk_mat->SetElement(2, 3, 0.49999);// .9 works, .85 fails, anything above works?
  vtk_mat->SetElement(3, 0, 0);
  vtk_mat->SetElement(3, 1, 0);
  vtk_mat->SetElement(3, 2, 0);
  vtk_mat->SetElement(3, 3, 1);

  vtkSmartPointer<vtkTransform> vtk_transform = vtkSmartPointer<vtkTransform>::New();
  vtk_transform->SetMatrix(vtk_mat);
  camera->ApplyTransform(vtk_transform);
  
  vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
  const double half_size = 50.;
  plane->SetOrigin(-half_size, -half_size, 0.);
  plane->SetPoint1(-half_size, half_size, 0.);
  plane->SetPoint2(half_size, -half_size, 0.);
  plane->SetNormal(0., 0., 1.);
  plane->Update();

  // For color and label.
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(plane->GetOutputPort());
  vtkSmartPointer<vtkActor> terrain_actor_ = vtkSmartPointer<vtkActor>::New();
  terrain_actor_->SetMapper(mapper);

  terrain_actor_->GetProperty()->SetColor(1.0, 0, 0);
  terrain_actor_->GetProperty()->LightingOff();

  renderer->AddActor(terrain_actor_);
  renderer->SetBackground(0, 0, 0);
  
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(window);
  window->Render();

  // Interactor can be used to change the image slightly manually.
  //renderWindowInteractor->Start();
  vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
  filter->SetInput(window);
#if VTK_MAJOR_VERSION < 8 || (VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION == 0)
    filter->SetMagnification(1);
#else
    filter->SetScale(1);
#endif
  filter->SetInputBufferTypeToRGBA();
  filter->Update();

  // Save output image.
  vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
  writer->SetInputData(filter->GetOutput());
  writer->SetFileName(argv[1]);
  writer->Write();

  // Compare pixel values.
  std::cout << "Reference pixel and color:" << std::endl;
  PrintValue(filter->GetOutput(),0,0);
  std::cout << "Pixels with a different color:" << std::endl;
  int* p0 = GetPixelValue(filter->GetOutput(), 0, 0);
  for (int i = 0 ; i < 640 ; i++)
  {
    for( int j = 0; j < 480; j++)
    {
      if (!EqualPixels(p0,GetPixelValue(filter->GetOutput(), i, j)))
      {
      PrintValue(filter->GetOutput(), i, j);
      }
    }
  }

  return 0;
}
