#include "SliceView.h"

SliceView::SliceView()
	: QObject()
{
	m_AxialRen = vtkSmartPointer<vtkRenderer>::New();
	m_AxialRenWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_AxialRenWinInt = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_AxialIntStyleImage = vtkSmartPointer<vtkInteractorStyleImage>::New();

	m_SagittalRen = vtkSmartPointer<vtkRenderer>::New();
	m_SagittalRenWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_SagittalRenWinInt = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_SagittalIntStyleImage = vtkSmartPointer<vtkInteractorStyleImage>::New();

	m_CoronalRen = vtkSmartPointer<vtkRenderer>::New();
	m_CoronalRenWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_CoronalRenWinInt = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_CoronalIntStyleImage = vtkSmartPointer<vtkInteractorStyleImage>::New();

	m_SpatialRen = vtkSmartPointer<vtkRenderer>::New();
	m_SpatialRenWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_SpatialRenWinInt = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_SpatialIntStyleTrackballCamera = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
}

SliceView::~SliceView()
{

}

void SliceView::SetWidget(QVTKWidget *AxiWidget,
						  QVTKWidget *SagWidget,
						  QVTKWidget *CorWidget,
						  QVTKWidget *SpatialWidget)
{
	AxiWidget->SetRenderWindow(m_AxialRenWin);
	SagWidget->SetRenderWindow(m_SagittalRenWin);
	CorWidget->SetRenderWindow(m_CoronalRenWin);
	SpatialWidget->SetRenderWindow(m_SpatialRenWin);

	m_AxialRenWin->AddRenderer(m_AxialRen);
	m_SagittalRenWin->AddRenderer(m_SagittalRen);
	m_CoronalRenWin->AddRenderer(m_CoronalRen);
	m_SpatialRenWin->AddRenderer(m_SpatialRen);

	// Setup interactor
	m_AxialRenWinInt->SetRenderWindow(m_AxialRenWin);
	m_AxialRenWinInt->Initialize();
	m_AxialRenWinInt->SetInteractorStyle(m_AxialIntStyleImage);

	m_SagittalRenWinInt->SetRenderWindow(m_SagittalRenWin);
	m_SagittalRenWinInt->Initialize();
	m_SagittalRenWinInt->SetInteractorStyle(m_SagittalIntStyleImage);

	m_CoronalRenWinInt->SetRenderWindow(m_CoronalRenWin);
	m_CoronalRenWinInt->Initialize();
	m_CoronalRenWinInt->SetInteractorStyle(m_CoronalIntStyleImage);

	m_SpatialRenWinInt->SetRenderWindow(m_SpatialRenWin);
	m_SpatialRenWinInt->Initialize();
	m_SpatialRenWinInt->SetInteractorStyle(m_SpatialIntStyleTrackballCamera);

	// Set backgroundColor
	m_AxialRen->SetBackground(0.5, 0.5, 1);
	m_SagittalRen->SetBackground(0.5, 0.5, 1);
	m_CoronalRen->SetBackground(0.5, 0.5, 1);
	m_SpatialRen->SetBackground(0.5, 0.5, 1);

	Render();
}

void SliceView::Render()
{
	m_AxialRenWin->Render();
	m_SagittalRenWin->Render();
	m_CoronalRenWin->Render();
	m_SpatialRenWin->Render();
}

void SliceView::ResetCamera()
{
	m_AxialRen->ResetCamera();
	m_SagittalRen->ResetCamera();
	m_CoronalRen->ResetCamera();
	m_SpatialRen->ResetCamera();
}