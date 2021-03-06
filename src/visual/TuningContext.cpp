#include "TuningContext.h"
#include "TuningCanvas.h"

#include "ColorTheme.h"

// http://stackoverflow.com/questions/7276826/c-format-number-with-commas
class comma_numpunct: public std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const {
        return ',';
    }

    virtual std::string do_grouping() const {
        return "\03";
    }
};

TuningContext::TuningContext(TuningCanvas *canvas, wxGLContext *sharedContext) :
        PrimaryGLContext(canvas, sharedContext) {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    comma_locale = std::locale(std::locale(), new comma_numpunct());
    freqStr.imbue(comma_locale);
}

void TuningContext::DrawBegin() {
    glClearColor(ThemeMgr::mgr.currentTheme->generalBackground.r, ThemeMgr::mgr.currentTheme->generalBackground.g, ThemeMgr::mgr.currentTheme->generalBackground.b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
}

void TuningContext::Draw(float r, float g, float b, float a, float p1, float p2) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBegin(GL_QUADS);
    glColor4f(r*0.5, g*0.5, b*0.5, a);
    glVertex2f(-1.0+p2*2.0, 1.0);
    glVertex2f(-1.0+p1*2.0, 1.0);
    glColor4f(r, g, b, a);
    glVertex2f(-1.0+p1*2.0, 0.0);
    glVertex2f(-1.0+p2*2.0, 0.0);

    glVertex2f(-1.0+p2*2.0, 0.0);
    glVertex2f(-1.0+p1*2.0, 0.0);
    glColor4f(r*0.5, g*0.5, b*0.5, a);
    glVertex2f(-1.0+p1*2.0, -1.0);
    glVertex2f(-1.0+p2*2.0, -1.0);
    glEnd();
    glDisable(GL_BLEND);
}

void TuningContext::DrawEnd() {
    glFlush();

    CheckGLError();
}

void TuningContext::DrawDemodFreqBw(long long freq, unsigned int bw, long long center) {
    GLint vp[4];
    glGetIntegerv( GL_VIEWPORT, vp);

    float viewHeight = (float) vp[3];
    float viewWidth = (float) vp[2];

    PrimaryGLContext::GLFontSize fontSize = GLFONT_SIZE16;

    int fontHeight = 16;

    if (viewWidth < 400) {
        fontSize = GLFONT_SIZE12;
        fontHeight = 12;
    }

    glColor3f(ThemeMgr::mgr.currentTheme->text.r, ThemeMgr::mgr.currentTheme->text.g, ThemeMgr::mgr.currentTheme->text.b);

    getFont(fontSize).drawString("Freq: ", -0.75, 0, fontHeight, GLFont::GLFONT_ALIGN_RIGHT, GLFont::GLFONT_ALIGN_CENTER);
    if (bw) {
        freqStr.str("");
        freqStr << std::fixed << freq << " Hz";
    } else {
        freqStr.str("---");
    }
    getFont(fontSize).drawString(freqStr.str(), -0.75, 0, fontHeight, GLFont::GLFONT_ALIGN_LEFT, GLFont::GLFONT_ALIGN_CENTER);


    getFont(fontSize).drawString("BW: ", -0.10, 0, fontHeight, GLFont::GLFONT_ALIGN_RIGHT, GLFont::GLFONT_ALIGN_CENTER);
    if (bw) {
        freqStr.str("");
        freqStr << std::fixed << bw << " Hz";
    } else {
        freqStr.str("---");
    }
    getFont(fontSize).drawString(freqStr.str(), -0.10, 0, fontHeight, GLFont::GLFONT_ALIGN_LEFT, GLFont::GLFONT_ALIGN_CENTER);


    getFont(fontSize).drawString("CTR: ", 0.50, 0, fontHeight, GLFont::GLFONT_ALIGN_RIGHT, GLFont::GLFONT_ALIGN_CENTER);
    if (center) {
        freqStr.str("");
        freqStr << std::fixed << center << " Hz";
    } else {
        freqStr.str("---");
    }
    getFont(fontSize).drawString(freqStr.str(), 0.50, 0, fontHeight, GLFont::GLFONT_ALIGN_LEFT, GLFont::GLFONT_ALIGN_CENTER);


    glColor3f(0.65, 0.65, 0.65);
    glBegin(GL_LINES);
    glVertex2f(-0.275, -1.0);
    glVertex2f(-0.275, 1.0);
    glVertex2f(0.275, -1.0);
    glVertex2f(0.275, 1.0);
    glEnd();

}

