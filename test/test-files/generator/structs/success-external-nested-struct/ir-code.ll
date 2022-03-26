; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%Socket = type { i32, i16, %NestedSocket }
%NestedSocket = type { i8*, i64 }

@0 = private unnamed_addr constant [11 x i8] c"Socket: %d\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %s = alloca %Socket, align 8
  %0 = alloca i16, align 2
  %1 = alloca %Socket, align 8
  store i32 0, i32* %result, align 4
  store i16 8080, i16* %0, align 2
  %2 = load i16, i16* %0, align 2
  %3 = call %Socket @"_f_struct(Socket)_openServerSocket_short"(i16 %2)
  store %Socket %3, %Socket* %1, align 8
  %4 = load %Socket, %Socket* %1, align 8
  store %Socket %4, %Socket* %s, align 8
  %5 = load %Socket, %Socket* %s, align 8
  %6 = getelementptr inbounds %Socket, %Socket* %s, i32 0, i32 0
  %7 = load i32, i32* %6, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %7)
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

declare %Socket @"_f_struct(Socket)_openServerSocket_short"(i16)

declare i32 @printf(i8*, ...)
