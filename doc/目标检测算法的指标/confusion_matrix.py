
#!/usr/bin/env python
# encoding: utf-8
"""
@author:tangjun
@desc: analysis the model infer result loaded from csv file, which has columns with
       manual code, predict code, code score.

安装库：
pip install scikit-learn  -i https://pypi.tuna.tsinghua.edu.cn/simple some-package
pip install pandas  -i https://pypi.tuna.tsinghua.edu.cn/simple some-package
pip install numpy  -i https://pypi.tuna.tsinghua.edu.cn/simple some-package

"""

import pandas as pd
import numpy as np
from sklearn.metrics import confusion_matrix
import os

import json
import pickle




class ResultAnalysis_tj(object):
    def __init__(self, read_path):
        # self.false_code='FALSE'
        # self.others_code='Others'
        self.read_path=read_path
        self.model_categories=[]

    def get_codename(self):
        code_name = []
        codes=os.listdir(self.read_path)
        for code in codes:
            if not code.endswith('.csv'):
                code_name.append(code)
        code_name=sorted(code_name)
        # if self.false_code not in code_name:
        #     code_name.append(self.false_code)
        # if self.others_code not in code_name:
        #     code_name.append(self.others_code)
        return code_name


    def get_true_pre_category(self):

        prediction_categories=[]
        true_categories=[]
        files=os.listdir(self.read_path)
        for file in files:
            if not file.endswith('.csv'):
                file_path=os.path.join(self.read_path,file)

                file_names=os.listdir(file_path)
                for file_name in file_names:
                    temp_path=os.path.join(file_path,file_name)
                    for end_name in os.listdir(temp_path):
                        if not end_name.endswith('.xml'):
                            true_categories.append(file)
                            prediction_categories.append(file_name)
        return true_categories,prediction_categories







    def process_confusion_matrix(self):
        true_categories,prediction_categories = self.get_true_pre_category()
        model_codes=self.get_codename()
        names=set(true_categories)
        for name in names:
            if name not in model_codes:
                model_codes.append(name)
        names=set(prediction_categories)
        for name in names:
            if name not in model_codes:
                model_codes.append(name)

        # model_codes=sorted(model_codes)
        self.model_categories=model_codes

        matrix = confusion_matrix(true_categories, prediction_categories, labels=self.model_categories,
                                  sample_weight=None)
        return matrix



    def save_confusion_matrix2csv(self):

        matrix=self.process_confusion_matrix()

        cm_df = self.cm2df(matrix, self.model_categories)
        precision_list = []
        recall_list = []
        for i in range(len(cm_df.columns)):
            precision = round(cm_df.iloc[i, i] / sum(cm_df[cm_df.columns[i]]), 4)
            recall = round(cm_df.iloc[i, i] / sum(cm_df.loc[cm_df.columns[i]]), 4)
            precision_list.append(precision)
            recall_list.append(recall)
        cm_df['recall'] = recall_list
        precision_list.append(None)
        cm_df.loc['precision'] = precision_list
        print(cm_df)
        import time
        out_name = str(time.strftime("%Y_%m%d_%H_%M", time.localtime()))
        save_file = os.path.join(self.read_path, 'matrix_'+str(out_name)+'.csv')

        cm_df.to_csv(save_file)

    @staticmethod
    def cm2df(conf_matrix, labels):
        df = pd.DataFrame()
        # rows
        for i, row_label in enumerate(labels):
            rowdata = {}
            # columns
            for j, col_label in enumerate(labels):
                rowdata[col_label] = conf_matrix[i, j]
            df = df.append(pd.DataFrame.from_dict({row_label: rowdata}, orient='index'))
        return df[labels]
















if __name__ == '__main__':


    read_path=r'C:\Users\vrc\Desktop\456'

    matrix_result=ResultAnalysis_tj(read_path)
    matrix_result.save_confusion_matrix2csv()
















