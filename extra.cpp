/* updateVBarRow, slow
void updateVBarRow(int x, int y) {
    if (VBARRED[y]) {
        return;
    }
    if (y >= DL_Y0 && y < DL_Y0+DL_HEIGHT) {
        int dly = y - DL_Y0;
        for (int dx=0; dx < VBAR_WIDTH; ++dx) {
            int tx = x + dx;

            //digital life
            if (tx >= DL_X0 && tx < DL_X0+DL_WIDTH) {
                int dlx = tx - DL_X0;
                std::string s = digitalLife[dly][dlx];
                if (s != " " && dx+stringWidth(s) <= VBAR_WIDTH) {
                    overwriteColWithU8(VSLIDE[y], tx, s);
                    continue;
                }
            }

            //fill
            overwriteColWithU8(VSLIDE[y], tx, "*");
        }
    }
    else {
        for (int tx=x; tx < x+VBAR_WIDTH; ++tx) {
            VSLIDE[y][tx] = '*';
        }
    }
    VBARRED[y] = true;
}
*/