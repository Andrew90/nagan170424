object ProtocolForm: TProtocolForm
  Left = 706
  Top = 279
  BorderStyle = bsSingle
  Caption = #1055#1088#1086#1090#1086#1082#1086#1083' '#1088#1072#1073#1086#1090#1099
  ClientHeight = 434
  ClientWidth = 266
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object mProtocol: TMemo
    Left = 11
    Top = 48
    Width = 241
    Height = 376
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    OnKeyDown = FormKeyDown
  end
  object cbProtocolToFile: TCheckBox
    Left = 24
    Top = 15
    Width = 209
    Height = 17
    Caption = #1047#1072#1087#1080#1089#1099#1074#1072#1090#1100' '#1087#1088#1086#1090#1086#1082#1086#1083' '#1074' '#1092#1072#1081#1083
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = cbProtocolToFileClick
    OnKeyDown = FormKeyDown
  end
end
