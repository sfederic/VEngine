#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <Windows.h>
#include <QKeyEvent>
#include "CoreSystem.h"
#include "Input.h"
#include <QMouseEvent>

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = Q_NULLPTR);

	void close()
	{
		gCoreSystem.bMainLoop = false;
	}

	void closeEvent(QCloseEvent* event) override
	{
		close();
	}

    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override
    {
        MSG* msg = (MSG*)message;

		switch (msg->message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);

		case WM_KEYDOWN:
			gInputSystem.StoreKeyDownInput(msg->wParam);

			//Close editor
			if (msg->wParam == VK_ESCAPE)
			{
				
			}

			break;

		case WM_KEYUP:
			gInputSystem.StoreKeyUpInput(msg->wParam);
			break;

		case WM_LBUTTONUP:
			gInputSystem.StoreMouseLeftUpInput(msg->wParam);
			break;

		case WM_LBUTTONDOWN:
			gInputSystem.StoreMouseLeftDownInput(msg->wParam);
			break;

		case WM_RBUTTONUP:
			gInputSystem.StoreMouseRightUpInput(msg->wParam);
			break;

		case WM_RBUTTONDOWN:
			gInputSystem.StoreMouseRightDownInput(msg->wParam);
			break;

		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(msg->wParam) < 0)
			{
				gInputSystem.StoreMouseWheelDown();
			}
			else
			{
				gInputSystem.StoreMouseWheelUp();
			}
			break;
		}

        return false;
    }

protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->key() == Qt::Key_R)
        {
            throw;
        }
    }

private:
    Ui::QtWidgetsApplication1Class ui;

};
