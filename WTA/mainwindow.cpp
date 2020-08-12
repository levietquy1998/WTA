#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <ctime>
#include <time.h>
int n = 0;
float Ta = 0.0001;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Begin->hide();
    ui->Random->hide();
    ui->checkBox->hide();
    //ui->label_5->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_N_clicked()
{
    n = ui->lineEdit->text().toInt();
    ui->Begin->show();
    ui->Random->show();
    ui->checkBox->show();
    ui->P->setColumnCount(n);
    ui->P->setRowCount(n);
    ui->U->setColumnCount(1);
    ui->U->setRowCount(n);
    ui->X->setColumnCount(n);
    ui->X->setRowCount(n);

    int p_size_h = (ui->P->size().height()-2)/n;
    int p_size_w = (ui->P->size().width()-2)/n;
    int x_size_h = (ui->X->size().height()-2)/n;
    int x_size_w = (ui->X->size().width()-2)/n;
    int u_size_h = (ui->U->size().height()-2)/n;
    int u_size_w = ui->U->size().width();
    for (int i=0; i<n; i++)
    {
        ui->P->setColumnWidth(i,p_size_w);
        ui->P->setRowHeight(i,p_size_h);
        ui->U->setRowHeight(i,u_size_h);
        ui->U->setColumnWidth(i,u_size_w);
        ui->X->setColumnWidth(i,x_size_w);
        ui->X->setRowHeight(i,x_size_h);
    }

}

void MainWindow::on_Random_clicked()
{

    for (int i=0; i<n*n; i++)
    {
        QTableWidgetItem *a = new QTableWidgetItem("0.5");
        if(i/n == i%n)
        {
            //a->setText("0.9");
            a->setText(random());
            ui->P->setItem(i/n,i%n,a);
            ui->P->item(i/n,i%n)->setTextAlignment(Qt::AlignCenter);
        }
        else
        {
            a->setText(random());
            ui->P->setItem(i/n,i%n,a);
            ui->P->item(i/n,i%n)->setTextAlignment(Qt::AlignCenter);
        }
    }

    for (int i=0; i<n; i++)
    {
        QTableWidgetItem *a = new QTableWidgetItem("0.5");
        a->setText(QString::number(i+1));
        ui->U->setItem(i,0,a);
        ui->U->item(i,0)->setTextAlignment(Qt::AlignCenter);
    }

}

QString MainWindow::random()
{
    float res = rand()%100 /(float)100;
    return QString::number(res);
}

int Check(double v_dot[])
{
    int check = 0;
    for (int i = 0 ; i < 2*n; i++)
    {
        if (abs(v_dot[i]) > 0.05) check++;
    }
    return check;
}

double Calculate_F(double x[], int u[], double** p)
{
    double* g = new double[n];
    //Cal_g
    for (int i = 0; i < n; i++)
    {
        g[i] = 1;
        for (int j = 0; j < n; j++)
        {
            g[i] = g[i] * pow((double)(1 - p[i][j]), (double)(x[i * n + j]));
        }
    }
    //Cal_F
    double F = 0;
    for (int i=0; i<n; i++)
    {
        F += g[i]*u[i];
    }
    return F;
}

void MainWindow ::Calculator()
{
    int* u = new int[n];
    int save_cycle = 0;
    for (int i=0; i<n;i++)
    {
        u[i] = ui->U->item(i,0)->text().toInt();
    }
    double** p = new double*[n];
    for (int i = 0; i < n; i++)
    {
        p[i] = new double[n];
    }
    for (int i = 0; i<n*n; i++)
    {
           p[i/n][i%n] = ui->P->item(i/n,i%n)->text().toDouble();
    }

    //Khai bao
    double* x = new double[n * n];
    double* w = new double[n * n];
    double* v = new double[2 * n];

    double* x_dot = new double[n * n];
    double* w_dot = new double[n * n];
    double* v_dot = new double[2 * n];

    double* g = new double[n];

    for (int i = 0; i < n * n; i++)
    {
        w[i] = 1;
        x[i] = 0;
        w_dot[i] = 0;
        x_dot[i] = 0;
    }
    for (int i = 0; i < 2 * n; i++)
    {
        v[i] = 1;
        v_dot[i] = 0;
    }
    clock_t start = clock();
        for (int k = 0; k < 1000000; k++)
        {
            //Cal_g
            for (int i = 0; i < n; i++)
            {
                g[i] = 1;
                for (int j = 0; j < n; j++)
                {
                    g[i] = g[i] * pow((double)(1 - p[i][j]), (double)(x[i * n + j]));
                }
            }
            //x_dot
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    x_dot[n * i + j] = -(u[i] * g[i] * log(1 - p[i][j]) - 0.5 * w[n * i + j] * w[n * i + j] + v[i] + v[n + j]);
                }
            }
            //w_dot
            for (int i = 0; i < n * n; i++)
            {
                w_dot[i] = -1 * w[i] * x[i];
            }
            //v_dot
            for (int i = 0; i < 2 * n; i++)
            {
                v_dot[i] = -1;
            }
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    v_dot[i] = v_dot[i] + x[n * i + j];
                    v_dot[n + i] = v_dot[n + i] + x[n * j + i];
                }
            }
            //xwv
            for (int i = 0; i < n * n; i++)
            {
                x[i] += x_dot[i] * Ta;
                w[i] += w_dot[i] * Ta;
            }
            for (int i = 0; i < 2 * n; i++)
            {
                v[i] += v_dot[i] * Ta;
            }
            if (k >= 100000 && Check(v_dot) ==0 && ui->checkBox->isChecked()== true)
            {
                save_cycle = k;
                break;
            }
        }
        for (int i = 0; i < n*n; i++)
        {
            if (x[i] > 0.5)
            {
                x[i] = 1;
                int a = i / n;
                int b = i % n;
                for (int j = 0; j < n; j++)
                {
                    if (a * n + j != i) x[a * n + j] = 0;
                }
                for (int j = 0; j < n; j++)
                {
                    if (b + n * j != i) x[b + n * j] = 0;
                }
            }
        }
        for (int i = 0; i < n * n; i++)
        {
            if (x[i] != 0 && x[i] != 1)
            {
                int Qcheck=0;
                int a = i / n;
                int b = i % n;
                for (int j = 0; j < n; j++)
                {
                    if (x[a * n + j] > x[i] ) Qcheck += 1;
                }
                for (int j = 0; j < n; j++)
                {
                    if (x[b + n * j] > x[i]) Qcheck += 1;
                }
                if (Qcheck == 0) x[i] = 1;
            }
        }
        for (int i = 0; i < n * n; i++)
        {
            if (x[i] != 1) x[i] = 0;
        }

        for (int i = 0; i < n * n; i++)
        {
            if (x[i] == 0)
            {
                int Qcheck=0;
                int a = i / n;
                int b = i % n;
                for (int j = 0; j < n; j++)
                {
                    if (x[a * n + j] != 0 ) Qcheck += 1;
                }
                for (int j = 0; j < n; j++)
                {
                    if (x[b + n * j] != 0 ) Qcheck += 1;
                }
                if (Qcheck == 0) x[i] = 1;
            }
        }

        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                ui->X->setItem(i,j,new QTableWidgetItem(QString::number(x[i*n+j])));
                if(x[i*n+j]==1)
                {
                    ui->X->item(i,j)->setBackgroundColor(Qt::red);
                }
                ui->X->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
        ui->label_5->setText("F = " + QString::number(Calculate_F(x,u,p))) ;
        delete []x;
        delete []w;
        delete []v;
        for (int i = 0; i < n; i++)
        {
            delete []p[i];
        }
        delete []p;
        delete []u;
        delete []g;
        delete []x_dot;
        delete []w_dot;
        delete []v_dot;
        if (save_cycle != 0) ui->time->setText( "Time = " + QString::number((double)(clock() - start) / CLOCKS_PER_SEC) + " s. Save time: " + QString::number(((double)(clock() - start) / CLOCKS_PER_SEC)*(1000000-save_cycle)/save_cycle) + "s");
        else ui->time->setText( "Time = " + QString::number((double)(clock() - start) / CLOCKS_PER_SEC) + " s");
}
void MainWindow::on_Begin_clicked()
{
  ui->Begin->setEnabled(false);
  Calculator();
  ui->Begin->setEnabled(true);
}



void MainWindow::on_Begin_pressed()
{
    ui->time->setText("Processing ...");
}
