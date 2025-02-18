void displayHeader(int pos) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(pos, 0);
  display.setTextSize(2);
  display.println(header);
}
