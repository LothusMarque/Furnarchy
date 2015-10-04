unit inetdate;
{Steven Kamradt <Steven@tempus.com>}
interface

USES
  Windows, SysUtils;

Function InetStdDateToDateTime(INetDate:String):tDateTime;
  {-Parse an Internet standard date time (RFC 822) into a LOCAL date time that
    can be used for computations in delphi.}

implementation

Function InetStdDateToDateTime(INetDate:String):tDateTime;
  {-Parse an Internet standard date time (RFC 822) into a LOCAL date time that
    can be used for computations in delphi.}
CONST
  Months : Array[1..12] of String[3] =
    ('JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC');
  DaysToMinutes = 24 * 60;
  MinutesToDays = 1 / DaysToMinutes;
VAR
  DayDigit, MonthName, YearDigit, Hour, Zone : String;
  DayNum,MonthNum,YearNum : Integer;
  TempConvertStr : String;
  IX : Integer;
  REs : Integer;
  TZI : tTimezoneinformation;
  AdjustedBias : LongInt;
  TempDate, TempTime: tDateTime;
  sHourPart, sMinPart : String;
  iHourPart, iMinPart : Integer;
BEGIN
  if Pos(',',InetDate) > 0 then // Remove the day of week if included
    Delete(InetDate,1,Pos(',',InetDate)+1);
  InetDate := Trim(InetDate); // Trim off any spaces
  DayDigit := Copy(InetDate,1,Pos(' ',InetDate)-1);  // Grab the Day of the month
  Delete(InetDate,1,Pos(' ',InetDate));
  MonthName := Copy(InetDate,1,Pos(' ',InetDate)-1); // Grab the month
  for IX := 1 to 12 do // Convert the month name to a number
    if Uppercase(MonthName) = Months[IX] then
      BEGIN
        MonthNum := IX;
        Break;
      END;
  Delete(InetDate,1,Pos(' ',InetDate));
  YearDigit := Copy(InetDate,1,Pos(' ',InetDate)-1); // Grab the year
  Delete(InetDate,1,Pos(' ',InetDate));
  Hour := Copy(InetDate,1,Pos(' ',InetDate)-1); // Grab the time
  Delete(InetDate,1,Pos(' ',InetDate));
  DayNum := StrToInt(DayDigit);
  YearNum := STrToInt(YearDigit);
  TempDate := EncodeDate(YearNum,MonthNum,DayNum);  // encode to a tdatetime rec
  TempTime := StrToTime(Hour);  // convert the time string to a tdatetime rec
  Result := TempDate + TempTime; // return the combined date and time
  ZOne := Uppercase(InetDate); // Grab the timezone portion
  RES := GetTimezoneINformation(TZI);
  AdjustedBias := tzi.Bias;
  if (res = 1) then // adjust if local is standard time
    AdjustedBias := AdjustedBias + tzi.StandardBias
  else if (Res = 2) then // adjust if local is daylight savings time
    AdjustedBias := AdjustedBias + tzi.DaylightBias;
  if (Zone = 'GMT') or (Zone = 'UT') or (Zone = 'Z') then // Adjust for GMT time
    RESULT := RESULT - (AdjustedBias * MinutesToDays)
  else if (Zone = 'EDT') then // adjust for Eastern Daylight Savings -4 GMT
    RESULT := RESULT - (240 * MinutesToDays)
  else if (Zone = 'EST') or (Zone = 'CDT') then // Adjust for EST or CDT -5 GMT
    RESULT := RESULT - (300 * MinutesToDays)
  else if (Zone = 'CST') or (ZONE = 'MDT') then // Adjust for CST or MDT -6 GMT
    Result := REsult - (360 * MinutesToDays)
  else if (Zone = 'MST') or (Zone = 'PDT') then // Adjust for MST or PDT -7 GMT
    Result := Result - (420 * MinutesToDays)
  else if (Zone = 'PST') then // Adjust for PST -8 GMT
    Result := Result - (480 * MinutesToDays)
  else if (Zone = 'M') then // Adjust for -12 GMT
    Result := Result - (720 * MinutesToDays)
  else if (Zone = 'A') then // Adjust for -1 GMT
    Result := Result - (60 * MinutesToDays)
  else if (Zone = 'N') then // Adjust for +1 GMT
    Result := Result + (60 * MinutesToDays)
  else if (Zone = 'Y') then // Adjust for +12 GMT
    Result := Result + (720 * MinutesToDays)
  else if (Length(Zone)=5) and (Zone[1] in ['-','+']) then
    BEGIN
      sHourPart := Copy(Zone,2,2);
      sMinPart := Copy(Zone,4,2);
      iHourPart := StrToInt(sHourPart);
      iMinPart := StrToInt(sMinPart);
      if Zone[1] = '+' then
        Result := Result - (((iHourPart * 60)+iMinPart) * MInutesToDays)
      else
        Result := Result + (((iHourPart * 60)+iMinPart) * MInutesToDays);
    END;
END;

end.
