; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [15 x i8] c"The age is: %d\00", align 1

define private i32 @_Z6getAgev() {
  %result = alloca i32, align 4
  %i = alloca i1, align 1
  store i1 false, ptr %i, align 1
  br i1 true, label %if.then.L3, label %if.else.L3

if.then.L3:                                       ; preds = %0
  store i32 20, ptr %result, align 4
  %1 = load i32, ptr %result, align 4
  ret i32 %1

if.else.L3:                                       ; preds = %0
  store i1 false, ptr %i, align 1
  br i1 false, label %if.then.L6, label %if.exit.L6

if.then.L6:                                       ; preds = %if.else.L3
  store i32 19, ptr %result, align 4
  br label %if.exit.L6

if.exit.L6:                                       ; preds = %if.then.L6, %if.else.L3
  br label %if.exit.L3

if.exit.L3:                                       ; preds = %if.exit.L6
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %age = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_Z6getAgev()
  store i32 %1, ptr %age, align 4
  %2 = load i32, ptr %age, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
