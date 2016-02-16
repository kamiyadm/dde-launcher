/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DISPLAYMODEFRAME_H
#define DISPLAYMODEFRAME_H

#include <QFrame>


class BaseCheckedButton;
class QButtonGroup;
class BaseFrame;
class ViewModeButton;
class QTimer;
class QPropertyAnimation;

class DisplayModeFrame : public QFrame
{
    Q_OBJECT
public:
    explicit DisplayModeFrame(QWidget *parent = 0);
    ~DisplayModeFrame();

    void initUI();
    void initConnect();
    int getCategoryDisplayMode();
    int getSortMode();

    BaseFrame* getButtonFrame();


signals:
    void categoryModeChanged(int mode);
    void sortModeChanged(int mode);
    void visibleChanged(bool isVisible);

public slots:
    void setCategoryDisplayMode(int mode);
    void setSortMode(int mode);
    void checkButton(int mode);
    void setViewMode(int mode);
    int getViewMode();

    void toggleButtonFrameByViewButton(bool flag);
    void toggleButtonFrameBySelf(bool flag);
    void showButtonFrame();
    void hideButtonFrame();

    void addTextShadow();

private:
    int m_categoryDisplayMode;
    int m_sortMode;
    int m_viewMode = 1;

    ViewModeButton* m_viewModeButton;
    BaseFrame* m_buttonFrame;
    BaseCheckedButton* m_nameButton;
    BaseCheckedButton* m_categoryIconButton;
    BaseCheckedButton* m_categoryNameButton;
    BaseCheckedButton* m_installTimeButton;
    BaseCheckedButton* m_useFrequencyButton;
    QButtonGroup* m_buttonGroup;
    QTimer* m_delayHideTimer;
    QTimer* m_delayShowTimer;
};

#endif // DISPLAYMODEFRAME_H
