; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Socket__int_short_NestedSocket = type { i32, i16, %_s__NestedSocket__string_long }
%_s__NestedSocket__string_long = type { i8*, i64 }

@0 = private unnamed_addr constant [17 x i8] c"Test string: %s\0A\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Socket: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %s = alloca i16, align 2
  %0 = alloca %_s__Socket__int_short_NestedSocket, align 8
  store i32 0, i32* %result, align 4
  store i16 8080, i16* %s, align 2
  %1 = load i16, i16* %s, align 2
  %2 = call %_s__Socket__int_short_NestedSocket @_f__void__openServerSocket__short(i16 %1)
  store %_s__Socket__int_short_NestedSocket %2, %_s__Socket__int_short_NestedSocket* %0, align 8
  %3 = load %_s__Socket__int_short_NestedSocket, %_s__Socket__int_short_NestedSocket* %0, align 8
  %4 = getelementptr inbounds %_s__Socket__int_short_NestedSocket, %_s__Socket__int_short_NestedSocket* %0, i32 0, i32 2
  %5 = load %_s__NestedSocket__string_long, %_s__NestedSocket__string_long* %4, align 8
  %6 = getelementptr inbounds %_s__NestedSocket__string_long, %_s__NestedSocket__string_long* %4, i32 0, i32 0
  %7 = load i8*, i8** %6, align 8
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), i8* %7)
  %9 = load %_s__Socket__int_short_NestedSocket, %_s__Socket__int_short_NestedSocket* %0, align 8
  %10 = getelementptr inbounds %_s__Socket__int_short_NestedSocket, %_s__Socket__int_short_NestedSocket* %0, i32 0, i32 0
  %11 = load i32, i32* %10, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0), i32 %11)
  %13 = load i32, i32* %result, align 4
  ret i32 %13
}

declare %_s__Socket__int_short_NestedSocket @_f__void__openServerSocket__short(i16)

declare i32 @printf(i8*, ...)
