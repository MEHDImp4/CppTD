#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QStyle>
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Weather Station");
    app.setApplicationVersion("1.0");
    
    // Set window icon (using standard system icon as fallback)
    app.setWindowIcon(app.style()->standardIcon(QStyle::SP_DesktopIcon));

    // Set modern font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);
    
    // Apply a stunning modern dark theme with gradients
    app.setStyleSheet(R"(
        * {
            font-family: 'Segoe UI', Arial, sans-serif;
            color: #ecf0f1;
        }
        
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1a1c2c, stop:1 #4a192c);
        }
        
        QWidget#centralWidget {
            background: transparent;
        }
        
        QGroupBox {
            font-weight: bold;
            font-size: 14px;
            border: 1px solid rgba(255, 255, 255, 0.15);
            border-radius: 12px;
            margin-top: 24px;
            padding: 24px 10px 10px 10px;
            background: rgba(0, 0, 0, 0.2);
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            top: 2px;
            padding: 0 5px;
            color: #ff7675;
            font-size: 16px;
        }
        
        QLineEdit {
            padding: 8px 12px;
            border: 1px solid rgba(255, 255, 255, 0.2);
            border-radius: 6px;
            font-size: 14px;
            background: rgba(255, 255, 255, 0.1);
            color: #ffffff;
            min-height: 25px;
        }
        
        QLineEdit:focus {
            border: 1px solid #ff7675;
            background: rgba(255, 255, 255, 0.15);
        }
        
        QLineEdit::placeholder {
            color: rgba(255, 255, 255, 0.4);
        }
        
        QPushButton {
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            min-width: 120px;
            padding: 10px 20px;
            color: white;
            background: #2d3436;
        }
        
        QPushButton:hover {
            margin-top: -2px;
        }
        
        QPushButton#addBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00b894, stop:1 #00cec9);
            color: #ffffff;
        }
        
        QPushButton#deleteBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff7675, stop:1 #d63031);
        }
        
        QPushButton#loadBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #0984e3, stop:1 #74b9ff);
        }
        
        QPushButton#saveBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #fdcb6e, stop:1 #e17055);
            color: #ffffffff;
        }
        
        QPushButton#saveBtn:hover {
            color: #ffffff;
        }
        
        QPushButton#statsBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #6c5ce7, stop:1 #a29bfe);
        }
        
        QTableWidget {
            border: none;
            border-radius: 8px;
            background: rgba(255, 255, 255, 0.05);
            alternate-background-color: rgba(255, 255, 255, 0.02);
            color: #ffffff;
            gridline-color: transparent;
        }
        
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.05);
        }
        
        QTableWidget::item:selected {
            background: rgba(255, 118, 117, 0.3);
            color: #ffffff;
        }
        
        QHeaderView::section {
            background: transparent;
            padding: 10px;
            border: none;
            border-bottom: 2px solid #ff7675;
            color: #ff7675;
            font-weight: bold;
            font-size: 14px;
        }
        
        QLabel {
            color: #dfe6e9;
        }

        QLabel#statLabel {
            background: rgba(0, 0, 0, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            padding: 8px;
            min-width: 80px;
            qproperty-alignment: AlignCenter;
        }
        
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0;
        }
        
        QScrollBar::handle:vertical {
            background: rgba(255, 255, 255, 0.2);
            border-radius: 4px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: rgba(255, 255, 255, 0.4);
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
    )");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
