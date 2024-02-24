; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Zero\00", align 1
@anon.string.1 = private unnamed_addr constant [4 x i8] c"One\00", align 1
@anon.string.2 = private unnamed_addr constant [4 x i8] c"Two\00", align 1
@anon.string.3 = private unnamed_addr constant [6 x i8] c"Three\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c"Four\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Five\00", align 1
@anon.string.6 = private unnamed_addr constant [4 x i8] c"Six\00", align 1
@anon.string.7 = private unnamed_addr constant [6 x i8] c"Seven\00", align 1
@anon.string.8 = private unnamed_addr constant [6 x i8] c"Eight\00", align 1
@anon.string.9 = private unnamed_addr constant [5 x i8] c"Nine\00", align 1
@anon.string.10 = private unnamed_addr constant [4 x i8] c"NaN\00", align 1
@printf.str.0 = private unnamed_addr constant [9 x i8] c"1 is %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [9 x i8] c"8 is %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [9 x i8] c"5 is %s\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [9 x i8] c"9 is %s\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [9 x i8] c"2 is %s\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [9 x i8] c"3 is %s\0A\00", align 1
@printf.str.6 = private unnamed_addr constant [9 x i8] c"4 is %s\0A\00", align 1
@printf.str.7 = private unnamed_addr constant [9 x i8] c"7 is %s\0A\00", align 1
@printf.str.8 = private unnamed_addr constant [9 x i8] c"6 is %s\0A\00", align 1
@printf.str.9 = private unnamed_addr constant [9 x i8] c"0 is %s\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [10 x i8] c"10 is %s\0A\00", align 1

define private ptr @_Z13digitToStringj(i32 %0) {
  %result = alloca ptr, align 8
  %number = alloca i32, align 4
  store i32 %0, ptr %number, align 4
  %2 = load i32, ptr %number, align 4
  switch i32 %2, label %switch.default.L13 [
    i32 0, label %switch.case.L3
    i32 1, label %switch.case.L4
    i32 2, label %switch.case.L5
    i32 3, label %switch.case.L6
    i32 4, label %switch.case.L7
    i32 5, label %switch.case.L8
    i32 6, label %switch.case.L9
    i32 7, label %switch.case.L10
    i32 8, label %switch.case.L11
    i32 9, label %switch.case.L12
  ]

switch.case.L3:                                   ; preds = %1
  ret ptr @anon.string.0

switch.case.L4:                                   ; preds = %1
  ret ptr @anon.string.1

switch.case.L5:                                   ; preds = %1
  ret ptr @anon.string.2

switch.case.L6:                                   ; preds = %1
  ret ptr @anon.string.3

switch.case.L7:                                   ; preds = %1
  ret ptr @anon.string.4

switch.case.L8:                                   ; preds = %1
  ret ptr @anon.string.5

switch.case.L9:                                   ; preds = %1
  ret ptr @anon.string.6

switch.case.L10:                                  ; preds = %1
  ret ptr @anon.string.7

switch.case.L11:                                  ; preds = %1
  ret ptr @anon.string.8

switch.case.L12:                                  ; preds = %1
  ret ptr @anon.string.9

switch.default.L13:                               ; preds = %1
  ret ptr @anon.string.10

switch.exit.L2:                                   ; No predecessors!
  %3 = load ptr, ptr %result, align 8
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call ptr @_Z13digitToStringj(i32 1)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = call ptr @_Z13digitToStringj(i32 8)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %3)
  %5 = call ptr @_Z13digitToStringj(i32 5)
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %5)
  %7 = call ptr @_Z13digitToStringj(i32 9)
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, ptr %7)
  %9 = call ptr @_Z13digitToStringj(i32 2)
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, ptr %9)
  %11 = call ptr @_Z13digitToStringj(i32 3)
  %12 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, ptr %11)
  %13 = call ptr @_Z13digitToStringj(i32 4)
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, ptr %13)
  %15 = call ptr @_Z13digitToStringj(i32 7)
  %16 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, ptr %15)
  %17 = call ptr @_Z13digitToStringj(i32 6)
  %18 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, ptr %17)
  %19 = call ptr @_Z13digitToStringj(i32 0)
  %20 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, ptr %19)
  %21 = call ptr @_Z13digitToStringj(i32 10)
  %22 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, ptr %21)
  %23 = load i32, ptr %result, align 4
  ret i32 %23
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
