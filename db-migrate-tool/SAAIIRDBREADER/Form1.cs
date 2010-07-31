using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

using System.IO;
using System.Data;
using System.Data.OleDb;
using System.IO;
using System.Text;


namespace SAAIIRDBREADER
{
    public partial class Form1 : Form
    {
        string path;
        string dbPw = "";
        string dbFile = "Cities_Proviemces.mdb";
        private string cnnStr;

        public Form1()
        {
            path = Environment.CurrentDirectory;
            string sepChar = Path.DirectorySeparatorChar.ToString();
            if (!path.EndsWith(sepChar))
                path += sepChar;

            dbFile = string.Concat(path, dbFile);  // dbFile = path + dbFile;
            cnnStr = string.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0};Jet OLEDB:Database Password={1};", dbFile, dbPw);

            InitializeComponent();
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Transfer(string tbl)
        {
            string sqlStr = string.Concat("SELECT * FROM ", tbl);

            try
            {
                OleDbConnection cnn = new OleDbConnection(cnnStr);

                cnn.Open();

                OleDbCommand cmd = new OleDbCommand(sqlStr, cnn);
                OleDbDataReader drr = cmd.ExecuteReader();

                string filePath = path + tbl + ".txt";
                string pattern = string.Empty;

                switch (tbl)
                {
                    case "Provience":
                        pattern = "db.Insert(\"provinces\", \"province, code\", 2, \"{0}\", \"{1}\");";
                        break;
                    case "Cities":
                        pattern = "db.Insert(\"cities\", \"city, code, prcode\", 3, \"{0}\", \"{1}\", \"{2}\");";
                        break;
                    case "Skills":
                        pattern = "db.Insert(\"skills\", \"title, code\", 2, \"{0}\", \"{1}\");";
                        break;
                    case "Standards":
                        pattern = "db.Insert(\"standards\", \"title, code, minedu, theorytm, practicaltm, interntm, projecttm, totaltm, startdt, prerequisite, skcode, sttype, abilities\", 13, \"{0}\", \"{1}\", \"{2}\", \"{3}\", \"{4}\", \"{5}\", \"{6}\", \"{7}\", \"{8}\", \"{9}\", \"{10}\", \"{11}\", \"{12}\");";
                        break;
                    default:
                        return;
                }

                using (StreamWriter sw = new StreamWriter(filePath, false, Encoding.UTF8))
                {
                    while (drr.Read())
                    {
                        string line = string.Empty;
                        
                        switch (tbl)
                        {
                            case "Provience":
                                line = string.Format(pattern, drr["prTitle"].ToString().Trim(), drr["prCode"].ToString().Trim());
                                break;
                            case "Cities":
                                line = string.Format(pattern, drr["ciTitle"].ToString().Trim(), drr["ciCode"].ToString().Trim(), drr["ciProvienceCode"].ToString().Trim());
                                break;
                            case "Skills":
                                line = string.Format(pattern, drr["skTitle"].ToString().Trim(), drr["skCode"].ToString().Trim());
                                break;
                            case "Standards":
                                line = string.Format(pattern, drr["stTitle"].ToString().Trim(), drr["stCode"].ToString().Trim(), drr["stMinEdjucation"].ToString().Trim(), drr["stTheory"].ToString().Trim(), drr["stPractical"].ToString().Trim(), drr["stIntern"].ToString().Trim(), drr["stProject"].ToString().Trim(), drr["stTotal"].ToString().Trim(), drr["stStart"].ToString().Trim(), drr["stPrerequisite"].ToString().Trim(), drr["stSkill"].ToString().Trim(), drr["stType"].ToString().Trim(), drr["stAbilities"].ToString().Trim());
                                break;
                        }

                        sw.WriteLine(line);
                    }
                }

                cnn.Close();
                drr.Close();

                cmd.Dispose();
                drr.Dispose();
                cnn.Dispose();

                cmd = null;
                drr = null;
                cnn = null;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error:\n\t" + ex.Message);
            }
            finally
            {
                tbl = null;
                sqlStr = null;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Transfer("Provience");
            Transfer("Cities");
            Transfer("Skills");
            Transfer("Standards");
        }
    }
}
