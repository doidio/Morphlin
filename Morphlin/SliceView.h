#ifndef SLICEVIEW_H
#define SLICEVIEW_H

#include "CustomQVTKWidget.h"

#include <QMessageBox>
#include <QObject>

#include <vtkCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

class SliceView : public QObject
{
	Q_OBJECT

public:
	SliceView();
	~SliceView();

	vtkSmartPointer<vtkRenderer>				m_AxialRen;
	vtkSmartPointer<vtkRenderWindow>			m_AxialRenWin;
	vtkSmartPointer<vtkRenderWindowInteractor>	m_AxialRenWinInt;
	vtkSmartPointer<vtkInteractorStyleImage>	m_AxialIntStyleImage;

	vtkSmartPointer<vtkRenderer>				m_SagittalRen;
	vtkSmartPointer<vtkRenderWindow>			m_SagittalRenWin;
	vtkSmartPointer<vtkRenderWindowInteractor>	m_SagittalRenWinInt;
	vtkSmartPointer<vtkInteractorStyleImage>	m_SagittalIntStyleImage;

	vtkSmartPointer<vtkRenderer>				m_CoronalRen;
	vtkSmartPointer<vtkRenderWindow>			m_CoronalRenWin;
	vtkSmartPointer<vtkRenderWindowInteractor>	m_CoronalRenWinInt;
	vtkSmartPointer<vtkInteractorStyleImage>	m_CoronalIntStyleImage;

	vtkSmartPointer<vtkRenderer>				m_SpatialRen;
	vtkSmartPointer<vtkRenderWindow>			m_SpatialRenWin;
	vtkSmartPointer<vtkRenderWindowInteractor>	m_SpatialRenWinInt;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_SpatialIntStyleTrackballCamera;

	void SetWidget(QVTKWidget *AxiWidget,
				   QVTKWidget *SagWidget,
				   QVTKWidget *CorWidget,
				   QVTKWidget *SpatialWidget);

	void ResetCamera();
	void Render();

private:

};

#endif // SLICEVIEW_H
