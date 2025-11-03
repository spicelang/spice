; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Apple\00", align 4
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Banana\00", align 4
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Orange\00", align 4
@printf.str.0 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4

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

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call ptr @_Z7getNamei(i32 0)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %1)
  %3 = call ptr @_Z7getNamei(i32 1)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %3)
  %5 = call ptr @_Z7getNamei(i32 2)
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr noundef %5)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
