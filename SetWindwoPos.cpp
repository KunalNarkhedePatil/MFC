
void CSetWindowPosAssignmentDlg::OnBnClickedButtonDemo()
{
	  // Get the button's current position
    CWnd* pButton = GetDlgItem(IDC_BUTTON_DEMO);

    if (pButton)
    {
        CRect rect;
        pButton->GetWindowRect(&rect);
        ScreenToClient(&rect);

        // Move the button 50 pixels down and right

		/*
		right top
		int newX = rect.left + 200;
        int newY = rect.top- 120;

		left top

		int newX = rect.left - 180;
        int newY = rect.top- 120;


		left bottom
		int newX = rect.left - 170;
        int newY = rect.top+ 110;

		right bottom
		 int newX = rect.left + 180;
        int newY = rect.top+ 150;
		*/

        int newX = rect.left + 180;
        int newY = rect.top+ 150;
        int width = rect.Width();
        int height = rect.Height();

        pButton->SetWindowPos(NULL, newX, newY, width, height, SWP_NOZORDER | SWP_NOSIZE);
    }
}
