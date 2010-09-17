#ifndef TARGETWIDGETFACTORY_H
#define TARGETWIDGETFACTORY_H

#include <olconf/module_config.hpp>
#include <olconf/register_group_config.hpp>
#include <olconf/register_config.hpp>
#include <olconf/flag_config.hpp>

#include <QLabel>
#include <QString>
#include <QSpinBox>

#include <vector>


class TargetWidgetFactory
{
private:


private:

    TargetWidgetFactory() { }

public:

    /** Creates widget for a given target configuration.
     *  The widget returned can be added to the QTablist in the
     *  main window.
     */
    static QWidget * createFrom(QWidget * parent, GovConfig const & config) {

        QVBoxLayout * layout = new QVBoxLayout();
        QWidget * container_widget = new QWidget(parent);

        ::std::vector<YAMLGen::OBLDC::ModuleConfig>::const_iterator mod_it;
        ::std::vector<YAMLGen::OBLDC::ModuleConfig>::const_iterator mod_end;
        mod_end = config.modules().end();
        for(mod_it = config.modules().begin(); mod_it != mod_end; ++mod_it) {
            QWidget * widget = TargetWidgetFactory::createFrom(container_widget, *mod_it);
            layout->addWidget(widget);
        }

        container_widget->setLayout(layout);
        return container_widget;
    }

    /** Creates widget group from module configuration.
     *  A module configuration may contain:
     *  - A list of register groups
     *  - Flag settings
     */
    static QWidget * createFrom(QWidget * parent, YAMLGen::OBLDC::ModuleConfig const & config) {
        ::std::vector<YAMLGen::OBLDC::RegisterGroupConfig>::const_iterator rg_it;
        ::std::vector<YAMLGen::OBLDC::RegisterGroupConfig>::const_iterator rg_end;
        rg_end = config.registers().end();
        QVBoxLayout * layout       = new QVBoxLayout();
        QWidget * container_widget = new QWidget(parent);

        for(rg_it = config.registers().begin(); rg_it != rg_end; ++rg_it) {
            QWidget * widget = TargetWidgetFactory::createFrom(container_widget, *rg_it);
            layout->addWidget(widget);
        }
        container_widget->setLayout(layout);
        return container_widget;
    }

    /** Creates widget for a given register group.
     *
     */
    static QWidget * createFrom(QWidget * parent, YAMLGen::OBLDC::RegisterGroupConfig const & config) {
        QGroupBox * group_box = new QGroupBox(parent);
        QVBoxLayout * layout  = new QVBoxLayout();

        group_box->setTitle(QString::fromStdString(config.description()));

        ::std::vector<YAMLGen::OBLDC::RegisterConfig>::const_iterator reg_it;
        ::std::vector<YAMLGen::OBLDC::RegisterConfig>::const_iterator reg_end;
        reg_end = config.registers().end();
        for(reg_it = config.registers().begin(); reg_it != reg_end; ++reg_it) {
            QWidget * widget = TargetWidgetFactory::createFrom(group_box, *reg_it);
            layout->addWidget(widget);
        }
        group_box->setLayout(layout);
        return group_box;
    }

    /** Creates widget for a given register.
     *
     */
    static QWidget * createFrom(QWidget * parent, YAMLGen::OBLDC::RegisterConfig const & config) {
        QWidget * container_widget = new QWidget(parent);
        QLabel * label             = new QLabel(QString::fromStdString(config.label()), container_widget);

        YAMLGen::OBLDC::WidgetConfig widget_conf = config.widget();
        ::std::string widget_class = widget_conf.classname();

        if(widget_class == "SpinBox") {
            QHBoxLayout * layout = new QHBoxLayout();
            layout->addWidget(label);
            QSpinBox * input;
            input = new QSpinBox(container_widget);
            input->setValue(widget_conf.default_value());

            QPushButton * commit_button;
            commit_button = new QPushButton("commit", container_widget);


            layout->addWidget(input);
            layout->addWidget(commit_button);
            container_widget->setLayout(layout);
        }
        else if(widget_class == "Checkbox") {
            QHBoxLayout * layout = new QHBoxLayout();
            layout->addWidget(label);
            QCheckBox * input;
            input = new QCheckBox(container_widget);

            layout->addWidget(input);
            container_widget->setLayout(layout);
        }
        else if(widget_class == "Slider") {
            QWidget * input_container_widget = new QWidget(container_widget);
            
            QVBoxLayout * vlayout = new QVBoxLayout();
            vlayout->addWidget(label);
            QHBoxLayout * hlayout = new QHBoxLayout();

            QSpinBox * spin_box;
            spin_box = new QSpinBox(input_container_widget);
            spin_box->setValue(widget_conf.default_value());

            QPushButton * commit_button;
            commit_button = new QPushButton("commit", input_container_widget);

            QSlider * input;
            input = new QSlider(Qt::Horizontal, input_container_widget);
            input->setSingleStep(widget_conf.step());
            input->setMinimum(widget_conf.min());
            input->setMaximum(widget_conf.max() || 100);
            input->setValue(widget_conf.default_value());
            vlayout->addWidget(input);

            hlayout->addWidget(spin_box);
            hlayout->addWidget(commit_button);

            input_container_widget->setLayout(hlayout);
            vlayout->addWidget(input_container_widget);
            container_widget->setLayout(vlayout);
        }

        return container_widget;
    }

    static QWidget * createFrom(QWidget * parent, YAMLGen::OBLDC::FlagConfig const & config) {
        return 0;
    }

};

#endif // TARGETWIDGETFACTORY_H
