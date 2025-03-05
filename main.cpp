#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/icon.h>
#include <cmath>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(480, 720), wxDEFAULT_FRAME_STYLE & ~(wxMAXIMIZE_BOX)) {
        #ifdef __WXMSW__
                SetIcon(wxIcon("db2px2.ico", wxBITMAP_TYPE_ICO));
        #else
                SetIcon(wxIcon("db2px2.png", wxBITMAP_TYPE_PNG));
        #endif
        wxPanel* panel = new wxPanel(this);
        wxFont boldFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        wxStaticBox* paramBox = new wxStaticBox(panel, wxID_ANY, "Parameters", wxPoint(10, 10), wxSize(445, 120));
        paramBox->SetFont(boldFont);

        wxStaticText* label1 = new wxStaticText(panel, wxID_ANY, "Maximum Volume (dB):", wxPoint(20, 40), wxSize(120, -1), wxALIGN_RIGHT);
        mdbInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150, 40), wxSize(50, -1), 0, wxTextValidator(wxFILTER_DIGITS));
        mdbInput->SetMaxLength(2);
        mdbInput->SetFont(boldFont);

        wxStaticText* label2 = new wxStaticText(panel, wxID_ANY, "List to (-dB):", wxPoint(20, 70), wxSize(120, -1), wxALIGN_RIGHT);
        ldbInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150, 70), wxSize(50, -1), 0, wxTextValidator(wxFILTER_DIGITS));
        ldbInput->SetMaxLength(3);
        ldbInput->SetFont(boldFont);

        wxStaticText* label3 = new wxStaticText(panel, wxID_ANY, "Precision (dB):", wxPoint(230, 40), wxSize(100, -1), wxALIGN_RIGHT);
        prcInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(340, 40), wxSize(50, -1), 0, wxTextValidator(wxFILTER_DIGITS));
        prcInput->SetMaxLength(2);
        prcInput->SetFont(boldFont);

        wxStaticText* label4 = new wxStaticText(panel, wxID_ANY, "VU Meter Size (px):", wxPoint(230, 70), wxSize(100, -1), wxALIGN_RIGHT);
        imglenInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(340, 70), wxSize(50, -1), 0, wxTextValidator(wxFILTER_DIGITS));
        imglenInput->SetMaxLength(4);
        imglenInput->SetFont(boldFont);

        wxFont buttonFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        wxButton* calcButton = new wxButton(panel, wxID_ANY, "Calculate", wxPoint(130, 100), wxSize(70, -1));
        calcButton->SetFont(buttonFont);
        wxButton* clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(250, 100), wxSize(70, -1));
        clearButton->SetFont(buttonFont);

        grid = new wxGrid(panel, wxID_ANY, wxPoint(15, 140), wxSize(450, 550));
        grid->CreateGrid(0, 2);
        grid->SetColLabelValue(0, "dB");
        grid->SetColLabelValue(1, "px");
        grid->SetColSize(0, 215);
        grid->SetColSize(1, 215);
        grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
        grid->SetDefaultCellBackgroundColour(panel->GetBackgroundColour());
        grid->EnableEditing(false);
        grid->EnableDragRowSize(false);
        grid->EnableDragColSize(false);
        grid->HideRowLabels();
        grid->EnableGridLines(true);
        grid->SetGridLineColour(wxColour(200, 200, 200));
        grid->SetSelectionMode(wxGrid::wxGridSelectRows);
        grid->Hide();

        paramBox->SetWindowStyle(paramBox->GetWindowStyle() | wxST_NO_AUTORESIZE);
        mdbInput->SetWindowStyle(mdbInput->GetWindowStyle() | wxTE_NO_VSCROLL);
        ldbInput->SetWindowStyle(ldbInput->GetWindowStyle() | wxTE_NO_VSCROLL);
        prcInput->SetWindowStyle(prcInput->GetWindowStyle() | wxTE_NO_VSCROLL);
        imglenInput->SetWindowStyle(imglenInput->GetWindowStyle() | wxTE_NO_VSCROLL);
        calcButton->SetWindowStyle(calcButton->GetWindowStyle() | wxBU_EXACTFIT);
        clearButton->SetWindowStyle(clearButton->GetWindowStyle() | wxBU_EXACTFIT);

        SetMinSize(wxSize(480, 720));
        SetMaxSize(wxSize(480, 720));

        calcButton->Bind(wxEVT_BUTTON, &MyFrame::OnCalculate, this);
        clearButton->Bind(wxEVT_BUTTON, &MyFrame::OnClear, this);
        grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &MyFrame::OnGridClick, this);
        grid->Bind(wxEVT_GRID_SELECT_CELL, &MyFrame::OnGridSelectCell, this);
    }

private:
    wxTextCtrl* mdbInput, * ldbInput, * prcInput, * imglenInput;
    wxGrid* grid;

    void OnCalculate(wxCommandEvent& event) {
        long mdb, ldb, prc, imglen;
        if (!mdbInput->GetValue().ToLong(&mdb) || !ldbInput->GetValue().ToLong(&ldb) ||
            !prcInput->GetValue().ToLong(&prc) || !imglenInput->GetValue().ToLong(&imglen)) {
            wxMessageBox("Please enter valid whole numbers!", "Error", wxOK | wxICON_ERROR);
            return;
        }
        if (mdb <= -ldb || prc <= 0 || imglen <= 0) {
            wxMessageBox("Max Volume must be greater than -List to, Precision and VU Size must be > 0!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        double dbRange = mdb - (-ldb);
        int rows = static_cast<int>(dbRange / prc) + 1;
        grid->ClearGrid();
        if (grid->GetNumberRows() > 0) grid->DeleteRows(0, grid->GetNumberRows());
        grid->AppendRows(rows);

        for (int i = 0; i < rows; i++) {
            double db = mdb - (i * prc);
            double x = std::round((std::pow(10, db / 40.0) / std::pow(10, mdb / 40.0)) * imglen);
            grid->SetCellValue(i, 0, wxString::Format("%.0f", db));
            grid->SetCellValue(i, 1, wxString::Format("%.0f", x));
        }

        grid->Show();
        Layout();
    }

    void OnClear(wxCommandEvent& event) {
        mdbInput->Clear();
        ldbInput->Clear();
        prcInput->Clear();
        imglenInput->Clear();
        grid->ClearGrid();
        if (grid->GetNumberRows() > 0) grid->DeleteRows(0, grid->GetNumberRows());
        grid->Hide();
        Layout();
    }

    void OnGridClick(wxGridEvent& event) {
        int row = event.GetRow();
        grid->SelectRow(row);
        event.Skip();
    }

    void OnGridSelectCell(wxGridEvent& event) {
        int row = event.GetRow();
        grid->SelectRow(row);
        event.Skip();
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() override {
        MyFrame* frame = new MyFrame("dB2px2");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);