#include "pythoneditorwidget.h"

#include <inviwo/core/util/logdistributor.h>

#include <Qt\qcommandlinkbutton.h>
#include <Qt\qsplitter.h>
#include <Qt\qfiledialog.h>
#include <Qt\qmessagebox.h>

namespace inviwo{

    PythonEditorWidget* PythonEditorWidget::pythonEditorWidget_ = 0;
    PythonEditorWidget* PythonEditorWidget::getPythonEditorWidget(){
        return pythonEditorWidget_;

    }


    PythonEditorWidget::PythonEditorWidget(InviwoMainWindow* mainWindow): InviwoDockWidget(tr("Python Editor"),mainWindow), script_(){
        setObjectName("PythonEditor");
        setAllowedAreas(Qt::AllDockWidgetAreas);
        InviwoApplication::getRef().registerFileObserver(this);

        QMenu *menu = mainWindow->menuBar()->addMenu("&Python");
        QAction *openAction = new QAction(QIcon(":/icons/python.png"),"&Python Editor",mainWindow);

        mainWindow->connect(openAction,SIGNAL(triggered(bool)),this,SLOT(show(void)));
        menu->addAction(openAction);

        buildWidget();
        resize(500,700);

        pythonEditorWidget_ = this;
    }

    void PythonEditorWidget::buildWidget(){
        setWindowIcon(QIcon(":/icons/python.png"));
    
        QWidget* content = new QWidget(this);
        QVBoxLayout *verticalLayout = new QVBoxLayout(content);

        ////Create Buttons
        QHBoxLayout *horizontalLayout = new QHBoxLayout();

        QToolButton* runButton = new QToolButton(content);
        QToolButton* newButton = new QToolButton(content);
        QToolButton* openButton = new QToolButton(content);
        QToolButton* saveButton = new QToolButton(content);
        QToolButton* saveAsButton = new QToolButton(content);
        QPushButton* clearOutputButton = new QPushButton(content);

        runButton->setIcon(QIcon(":/icons/python.png"));
        runButton->setToolTip("Compile and run");

        newButton->setIcon(QIcon(":/icons/new.png"));
        newButton->setToolTip("New file");

        openButton->setIcon(QIcon(":/icons/open.png"));
        openButton->setToolTip("Open Python script");

        saveButton->setIcon(QIcon(":/icons/save.png"));
        saveButton->setToolTip("Save");

        saveAsButton->setIcon(QIcon(":/icons/saveas.png"));
        saveAsButton->setToolTip("Save as");
        
        clearOutputButton->setText("Clear Output");

        
        
        QFrame* line = new QFrame(content);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);


        horizontalLayout->addWidget(runButton);
        horizontalLayout->addWidget(line);
        horizontalLayout->addWidget(newButton);
        horizontalLayout->addWidget(openButton);
        horizontalLayout->addWidget(saveButton);
        horizontalLayout->addWidget(saveAsButton);
        horizontalLayout->addWidget(line);
        horizontalLayout->addWidget(clearOutputButton);

        QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayout->addItem(horizontalSpacer);

        
        //Done creating buttons

        
        QSplitter* splitter = new  QSplitter(content);
        splitter->setOrientation(Qt::Vertical);

        pythonCode_ = new QTextEdit(content);
        pythonCode_->setObjectName("pythonEditor");
        
        codeChanged_ = false;
        setDefaultText();       

        pythonOutput_ = new QTextEdit(content);
        pythonOutput_->setObjectName("pythonConsole");
        pythonOutput_->setReadOnly(true);
        pythonOutput_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        
        verticalLayout->addLayout(horizontalLayout);
        
        splitter->addWidget(pythonCode_);
        splitter->addWidget(pythonOutput_);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 0);

        verticalLayout->addWidget(splitter);

        setWidget(content);
        
        connect(pythonCode_,SIGNAL(textChanged()),this,SLOT(onTextChange()));
        connect(runButton,SIGNAL(clicked()),this,SLOT(run()));
        connect(newButton,SIGNAL(clicked()),this,SLOT(setDefaultText()));
        connect(openButton,SIGNAL(clicked()),this,SLOT(open()));
        connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
        connect(saveAsButton,SIGNAL(clicked()),this,SLOT(saveAs()));
        connect(clearOutputButton,SIGNAL(clicked()),this,SLOT(clearOutput()));
        
    }


    PythonEditorWidget::~PythonEditorWidget(){}

    void PythonEditorWidget::appendToOutput(std::string msg){
        pythonOutput_->append(msg.c_str());
    }

    void PythonEditorWidget::fileChanged(std::string fileName){
        std::string msg = "The file " + UrlParser::getFileNameWithExtension(scriptFileName_) + " has been modified outside of Inwivo, do you want to reload its contents";
        int ret = QMessageBox::question(this,"Python Editor",msg.c_str(),"Yes","No");
        if(ret == 0){//yes
            readFile();
        }else{
            codeChanged_ = true; //set code change to true so that we can quick save without making more changes
        }
    }

    void PythonEditorWidget::save(){
        if(scriptFileName_.size()==0){
            saveAs();
        }else{
            stopFileObservation(scriptFileName_);
            std::ofstream file(scriptFileName_);
            file << pythonCode_->toPlainText().toLocal8Bit().constData();
            file.close();
            startFileObservation(scriptFileName_);

            std::string logMsg = "Python Script saved("+scriptFileName_+")";
            LogInfo("Python Script saved("<<scriptFileName_<<")");

            if(codeChanged_){
                const std::string source = pythonCode_->toPlainText().toLocal8Bit().constData();
                script_.setSource(source);
                clearOutput();
                script_.compile();
            }
            codeChanged_ = false;
        }
    }

    void PythonEditorWidget::readFile(){
        std::ifstream file(scriptFileName_);
        
        std::string text((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        file.close();

        pythonCode_->setText(text.c_str());
        codeChanged_ = true;
    }

    void PythonEditorWidget::saveAs(){
        QStringList extensions;
        extensions << "Python Script (*.py)";

        QFileDialog saveFileDialog(this, tr("Save Python Script ..."));
        //TODO consider added scripts folder and use getPath instead of getBasePath
        //or to use workspace directory
        saveFileDialog.setDirectory(QDir((InviwoApplication::getPtr()->getBasePath() + "/data/scripts").c_str()));
        saveFileDialog.setFileMode(QFileDialog::AnyFile);
        saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
        saveFileDialog.setConfirmOverwrite(true);
        saveFileDialog.setNameFilters(extensions);

        if(saveFileDialog.exec()){
            stopFileObservation(scriptFileName_);
            QString path = saveFileDialog.selectedFiles().at(0);
            if (!path.endsWith(".py")) path.append(".py");
            scriptFileName_ = path.toLocal8Bit().constData();
            save();
        }
    }

    void PythonEditorWidget::open(){
        if(codeChanged_){
            int ret = QMessageBox::information(this,"Python Editor","Do you want to save unsaved changes?","Save","Discard","Cancel");
            if(ret == 0)
                save();
            if(ret == 2) //Cancel
                return;
        }

        QStringList extensions;
        extensions << "Python Script (*.py)";

        QFileDialog openFileDialog(this, tr("Open Python Script ..."));
        //TODO consider added scripts folder and use getPath instead of getBasePath
        openFileDialog.setDirectory(QDir((InviwoApplication::getPtr()->getBasePath() + "/data/scripts").c_str()));
        openFileDialog.setFileMode(QFileDialog::AnyFile);
        openFileDialog.setNameFilters(extensions);

        if (openFileDialog.exec()) {
            stopFileObservation(scriptFileName_);
            scriptFileName_ = openFileDialog.selectedFiles().at(0).toLocal8Bit().constData();
            startFileObservation(scriptFileName_);
            readFile();
        }
    }

    void PythonEditorWidget::run(){
        if(codeChanged_){ //save and recompile
            if(scriptFileName_.size()!=0)
                save();
            const std::string source = pythonCode_->toPlainText().toLocal8Bit().constData();
            script_.setSource(source);
            if(!script_.compile()){
                appendToOutput(script_.getLog());
                return;
            }
        }
        clearOutput();
        if(!script_.run()){
            appendToOutput(script_.getLog());
        }
    }
    
    void PythonEditorWidget::show(){
        setFloating(true);
        setVisible(true);
    }

    void PythonEditorWidget::setDefaultText(){
        if(codeChanged_){
            int ret = QMessageBox::information(this,"Python Editor","Do you want to save unsaved changes?","Save","Discard Changes","Cancel");
            if(ret == 0)
                save();
            else if(ret == 2) //cancel
                return;
        }
        pythonCode_->setText("# Inviwo Python script \nimport inviwo \n\ninviwo.info() \n");
        stopFileObservation(scriptFileName_);
        scriptFileName_ = "";
        codeChanged_ = true;
    }

    void PythonEditorWidget::clearOutput(){
        pythonOutput_->setText("");
    }

    void PythonEditorWidget::onTextChange(){
        codeChanged_ = true;
    }



} // namespace