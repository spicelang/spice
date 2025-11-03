; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 4
@printf.str.1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 4
@printf.str.2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 4
@printf.str.3 = private unnamed_addr constant [5 x i8] c"Else\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  br i1 false, label %if.then.L2, label %if.else.L2

if.then.L2:                                       ; preds = %0
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  br label %if.exit.L2

if.else.L2:                                       ; preds = %0
  br i1 false, label %if.then.L4, label %if.else.L4

if.then.L4:                                       ; preds = %if.else.L2
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  br label %if.exit.L4

if.else.L4:                                       ; preds = %if.else.L2
  br i1 false, label %if.then.L6, label %if.else.L6

if.then.L6:                                       ; preds = %if.else.L4
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  br label %if.exit.L6

if.else.L6:                                       ; preds = %if.else.L4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  br label %if.exit.L6

if.exit.L6:                                       ; preds = %if.else.L6, %if.then.L6
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %if.exit.L6, %if.then.L4
  br label %if.exit.L2

if.exit.L2:                                       ; preds = %if.exit.L4, %if.then.L2
  %5 = load i32, ptr %result, align 4
  ret i32 %5
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
