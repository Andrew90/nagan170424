object DM: TDM
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 236
  Width = 491
  object ADOCon: TADOConnection
    CommandTimeout = 3
    ConnectionTimeout = 5
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    Left = 224
    Top = 8
  end
  object DataDefect: TDataSource
    DataSet = DefectTable
    Left = 24
    Top = 160
  end
  object DataTube: TDataSource
    DataSet = TubeTable
    Left = 120
    Top = 160
  end
  object TubeTable: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'InfoTube'
    Left = 120
    Top = 112
    object TubeTableID_Tube: TLargeintField
      FieldName = 'ID_Tube'
      ReadOnly = True
    end
    object TubeTableResult: TWideStringField
      FieldName = 'Result'
      FixedChar = True
      Size = 2
    end
    object TubeTableSolidGroup: TWideStringField
      FieldName = 'SolidGroup'
      Size = 2
    end
    object TubeTableDate: TDateTimeField
      FieldName = 'Date'
    end
    object TubeTableMinThickness: TFloatField
      FieldName = 'MinThickness'
    end
    object TubeTableCrossResult: TStringField
      FieldName = 'CrossResult'
      FixedChar = True
      Size = 2
    end
    object TubeTableLineResult: TStringField
      FieldName = 'LineResult'
      FixedChar = True
      Size = 2
    end
    object TubeTableLength: TFloatField
      FieldName = 'Length'
    end
    object TubeTableThicknessResult: TStringField
      FieldName = 'ThicknessResult'
      FixedChar = True
      Size = 2
    end
    object TubeTableTypeSize: TIntegerField
      FieldName = 'TypeSize'
    end
    object TubeTableisEtalon: TBooleanField
      FieldName = 'isEtalon'
    end
    object TubeTableDefectoscoperID: TIntegerField
      FieldName = 'DefectoscoperID'
    end
    object TubeTableTubeGroup: TIntegerField
      FieldName = 'TubeGroup'
    end
    object TubeTableCustomerID: TIntegerField
      FieldName = 'CustomerID'
    end
    object TubeTableNumberTube: TLargeintField
      FieldName = 'NumberTube'
    end
  end
  object DefectTable: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'Defect'
    Left = 24
    Top = 112
    object DefectTableID_Tube: TLargeintField
      FieldName = 'ID_Tube'
    end
    object DefectTableZone: TWordField
      FieldName = 'Zone'
    end
    object DefectTableCrossDefect: TFloatField
      FieldName = 'CrossDefect'
    end
    object DefectTableLineDefect: TFloatField
      FieldName = 'LineDefect'
    end
  end
  object QueryThick: TADOQuery
    Connection = ADOCon
    Parameters = <
      item
        Name = 'Par'
        DataType = ftVariant
        Value = 0
      end>
    Left = 200
    Top = 112
  end
  object Find: TADOQuery
    Connection = ADOCon
    Parameters = <
      item
        Name = 'par'
        DataType = ftWideMemo
        Size = 1
        Value = ''
      end
      item
        Name = 'par1'
        DataType = ftInteger
        Value = 0
      end
      item
        Name = 'ParSG0'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParResult0'
        Size = -1
        Value = Null
      end
      item
        Name = 'Begin'
        DataType = ftDateTime
        Size = -1
        Value = Null
      end
      item
        Name = 'End'
        DataType = ftDateTime
        Size = -1
        Value = Null
      end
      item
        Name = 'ParSG1'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParSG2'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParResult1'
        Size = -1
        Value = Null
      end
      item
        Name = 'ParResult2'
        Size = -1
        Value = Null
      end>
    Left = 384
    Top = 112
  end
  object DataFind: TDataSource
    DataSet = Find
    Left = 392
    Top = 160
  end
  object TEtalons: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'SGEtalon'
    Left = 200
    Top = 160
    object TEtalonsName: TMemoField
      FieldName = 'Name'
      BlobType = ftWideString
      Size = 5
    end
    object TEtalonsTypeSize: TSmallintField
      FieldName = 'TypeSize'
    end
    object TEtalonsp1: TSmallintField
      FieldName = 'p1'
    end
    object TEtalonsp2: TSmallintField
      FieldName = 'p2'
    end
    object TEtalonsp3: TSmallintField
      FieldName = 'p3'
    end
    object TEtalonsp4: TSmallintField
      FieldName = 'p4'
    end
    object TEtalonsp5: TSmallintField
      FieldName = 'p5'
    end
    object TEtalonsp6: TSmallintField
      FieldName = 'p6'
    end
    object TEtalonsp7: TSmallintField
      FieldName = 'p7'
    end
    object TEtalonsp8: TSmallintField
      FieldName = 'p8'
    end
    object TEtalonsDate: TDateTimeField
      FieldName = 'Date'
    end
  end
  object DataEtalons: TDataSource
    DataSet = QEtalons
    Left = 296
    Top = 160
  end
  object QEtalons: TADOQuery
    Connection = ADOCon
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.SGEtalon')
    Left = 296
    Top = 112
  end
  object tTemp: TADOTable
    Connection = ADOCon
    CursorType = ctStatic
    TableName = 'SGEtalon'
    Left = 320
    Top = 8
    object tTempID: TAutoIncField
      FieldName = 'ID'
      ReadOnly = True
    end
    object tTempName: TWideStringField
      FieldName = 'Name'
      Size = 5
    end
    object tTempp1: TSmallintField
      FieldName = 'p1'
    end
    object tTempp2: TSmallintField
      FieldName = 'p2'
    end
    object tTempp3: TSmallintField
      FieldName = 'p3'
    end
    object tTempp4: TSmallintField
      FieldName = 'p4'
    end
    object tTempp5: TSmallintField
      FieldName = 'p5'
    end
    object tTempp6: TSmallintField
      FieldName = 'p6'
    end
    object tTempp7: TSmallintField
      FieldName = 'p7'
    end
    object tTempp8: TSmallintField
      FieldName = 'p8'
    end
    object tTempTypeSize: TSmallintField
      FieldName = 'TypeSize'
    end
    object tTempDate: TDateTimeField
      FieldName = 'Date'
    end
  end
  object Table: TADOTable
    Left = 88
    Top = 24
  end
end
