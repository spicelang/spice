; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Apple\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Banana\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Orange\00", align 1
@printf.str.0 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define private ptr @_Z7getNamei(i32 %0) {
  %result = alloca ptr, align 8
  %input = alloca i32, align 4
  store i32 %0, ptr %input, align 4
  %2 = load i32, ptr %input, align 4
  switch i32 %2, label %switch.exit.L8 [
    i32 0, label %switch.case.L9
    i32 1, label %switch.case.L10
    i32 2, label %switch.case.L11
  ]

switch.case.L9:                                   ; preds = %1
  ret ptr @anon.string.0

switch.case.L10:                                  ; preds = %1
  ret ptr @anon.string.1

switch.case.L11:                                  ; preds = %1
  ret ptr @anon.string.2

switch.exit.L8:                                   ; preds = %1
  %3 = load ptr, ptr %result, align 8
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call ptr @_Z7getNamei(i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = call ptr @_Z7getNamei(i32 1)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %3)
  %5 = call ptr @_Z7getNamei(i32 2)
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %5)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
