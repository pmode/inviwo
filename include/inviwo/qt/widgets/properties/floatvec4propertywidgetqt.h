#ifndef IVW_FLOATVEC4PROPERTYWIDGETQT_H
#define IVW_FLOATVEC4PROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>
//Core
#include <inviwo/core/properties/vectorproperties.h>


namespace inviwo {

class IVW_QTWIDGETS_API FloatVec4PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatVec4PropertyWidgetQt(FloatVec4Property* property);

    void updateFromProperty();

private:
    FloatVec4Property* property_;
    FloatSliderWidgetQt* sliderX_;
    FloatSliderWidgetQt* sliderY_;
    FloatSliderWidgetQt* sliderZ_;
	FloatSliderWidgetQt* sliderW_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValueFromSlider();
    void setPropertyValueFromSpinBox();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
    void showContextMenuZ(const QPoint& pos);
    void showContextMenuW(const QPoint& pos);

};

} // namespace

#endif // IVW_FLOATVEC3PROPERTYWIDGETQT_H