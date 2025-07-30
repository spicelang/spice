; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Input is at least 5.\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [22 x i8] c"Input is at least 4.\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [22 x i8] c"Input is at least 3.\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [22 x i8] c"Input is at least 2.\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [22 x i8] c"Input is at least 1.\0A\00", align 4

define private void @_Z12isBiggerThani(i32 %0) {
  %input = alloca i32, align 4
  store i32 %0, ptr %input, align 4
  %2 = load i32, ptr %input, align 4
  switch i32 %2, label %switch.exit.L2 [
    i32 5, label %switch.case.L3
    i32 4, label %switch.case.L7
    i32 3, label %switch.case.L11
    i32 2, label %switch.case.L15
    i32 1, label %switch.case.L19
  ]

switch.case.L3:                                   ; preds = %1
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  br label %switch.case.L7

switch.case.L7:                                   ; preds = %1, %switch.case.L3
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  br label %switch.case.L11

switch.case.L11:                                  ; preds = %1, %switch.case.L7
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  br label %switch.case.L15

switch.case.L15:                                  ; preds = %1, %switch.case.L11
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  br label %switch.case.L19

switch.case.L19:                                  ; preds = %1, %switch.case.L15
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4)
  br label %switch.exit.L2

switch.exit.L2:                                   ; preds = %switch.case.L19, %1
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z12isBiggerThani(i32 3)
  call void @_Z12isBiggerThani(i32 5)
  call void @_Z12isBiggerThani(i32 1)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
