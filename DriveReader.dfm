object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 418
  ClientWidth = 791
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 167
    Width = 166
    Height = 13
    Caption = #1057#1095#1080#1090#1072#1090#1100' '#1089' '#1091#1082#1072#1079#1072#1085#1085#1086#1075#1086' '#1082#1083#1072#1089#1090#1077#1088#1072':'
  end
  object Label2: TLabel
    Left = 8
    Top = 213
    Width = 119
    Height = 13
    Caption = #1055#1086' '#1091#1082#1072#1079#1072#1085#1085#1099#1081' '#1082#1083#1072#1089#1090#1077#1088':'
  end
  object ListBoxDrive: TListBox
    Left = 8
    Top = 8
    Width = 166
    Height = 97
    ItemHeight = 13
    TabOrder = 0
    OnClick = ListBoxClick
  end
  object ButtonDriveInfo: TButton
    Left = 8
    Top = 120
    Width = 166
    Height = 41
    Caption = #1048#1085#1092#1086#1088#1084#1072#1094#1080#1103' '#1086' '#1076#1080#1089#1082#1077
    TabOrder = 1
    OnClick = ShowDriveInfo
  end
  object ButtonReadCluster: TButton
    Left = 8
    Top = 263
    Width = 166
    Height = 41
    Caption = #1057#1095#1080#1090#1072#1090#1100' '#1082#1083#1072#1089#1090#1077#1088#1099
    TabOrder = 2
    OnClick = ClustersRead
  end
  object ButtonSave: TButton
    Left = 8
    Top = 369
    Width = 166
    Height = 41
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1072#1082
    TabOrder = 3
    OnClick = SaveAs
  end
  object Memo1: TMemo
    Left = 180
    Top = 8
    Width = 425
    Height = 402
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssVertical
    TabOrder = 4
  end
  object EditFrom: TEdit
    Left = 8
    Top = 186
    Width = 166
    Height = 21
    TabOrder = 5
    TextHint = 'From'
  end
  object EditTo: TEdit
    Left = 8
    Top = 232
    Width = 166
    Height = 21
    TabOrder = 6
    TextHint = 'To'
  end
  object ButtonSearchTxt: TButton
    Left = 8
    Top = 310
    Width = 166
    Height = 41
    Caption = #1053#1072#1081#1090#1080' '#1087#1077#1088#1074#1099#1081' txt '#1092#1072#1081#1083#1099' '#1074' '#1060#1057
    TabOrder = 7
    OnClick = SearchTxt
  end
  object SaveTextFileDialog1: TSaveTextFileDialog
    Left = 720
    Top = 8
  end
end
