#ifndef PCH_H
#define PCH_H
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#endif // PCH_H

#ifndef MORPHLIN_H
#define MORPHLIN_H

// project headers
#include "ui_Morphlin.h"
#include "ImageKit.h"
#include "SliceView.h"

// Qt headers
#include <QMessageBox>
#include <QtWidgets/QMainWindow>

// VTK headers

class Morphlin : public QMainWindow
{
	Q_OBJECT

public:
	Morphlin();
	~Morphlin();

public slots:
	// action slots
	void OnLoadDICOM();
	void OnImageKit();

	// sliceView ctrls
	void SetupSliceViewBar();
	void OnScrollBarToSpinBox(int);
	void OnSpinBoxToScrollBar(double);
	void OnChangeSliceIndex(int axi, int sag, int cor);

private:
	Ui::MorphlinClass ui;

	ImageKit *m_ImageKit;
	SliceView *m_SliceView;

	// Turn to FALSE to cut off connection between ScrollBar and SpinBox
	bool m_bScrollBarSpinBoxConnect = true;
};

#endif // MORPHLIN_H
