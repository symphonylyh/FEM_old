import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from test0 import Ui_MainWindow

class TestApp(Ui_MainWindow):
    def __init__(self, dialog):
        Ui_MainWindow.__init__(self)
        self.setupUi(dialog)

        self.addButton.clicked.connect(self.addRow)
        self.sumColButton.clicked.connect(self.sumCol)

    def addRow(self):
        #Retrieve text from QLineEdit
        # Retrieve text from QLineEdit
        x = self.x_input.text()
        y = self.y_input.text()
        z = self.z_input.text()
        # Create a empty row at bottom of table
        numRows = self.tableWidget.rowCount()
        self.tableWidget.insertRow(numRows)
        # Add text to the row
        self.tableWidget.setItem(numRows, 0, QtWidgets.QTableWidgetItem(0))
        self.tableWidget.setItem(numRows, 1, QtWidgets.QTableWidgetItem(0))
        self.tableWidget.setItem(numRows, 2, QtWidgets.QTableWidgetItem(0))

    def sumCol(self):
        #Create lists to hold values from each column
        xlist = []
        ylist = []
        zlist = []
        numRows = self.tableWidget.rowCount()
        for row in range(numRows):
            #Retreive item from the cell
            xitem = self.tableWidget.item(row, 0)
            yitem = self.tableWidget.item(row, 1)
            zitem = self.tableWidget.item(row, 2)
            #item to text
            x = float(xitem.text())
            y = float(yitem.text())
            z = float(zitem.text())
            #Add to list
            xlist.append(x)
            ylist.append(y)
            zlist.append(z)
        #Sum values in the column
        xsum = str(sum(xlist))
        ysum = str(sum(ylist))
        zsum = str(sum(zlist))
        #Display sum
        self.xdisp.setText(xsum)
        self.ydisp.setText(ysum)
        self.zdisp.setText(zsum)

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    dialog = QtWidgets.QMainWindow()

    test0 = TestApp(dialog)

    dialog.show()
    sys.exit(app.exec_())