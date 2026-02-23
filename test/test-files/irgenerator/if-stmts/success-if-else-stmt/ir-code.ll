; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 4
@printf.str.1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 4
@printf.str.2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 4
@printf.str.3 = private unnamed_addr constant [5 x i8] c"Else\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %condition1 = alloca i1, align 1
  %condition2 = alloca i1, align 1
  %condition3 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store i1 false, ptr %condition1, align 1
  store i1 false, ptr %condition2, align 1
  store i1 false, ptr %condition3, align 1
  %1 = load i1, ptr %condition1, align 1
  br i1 %1, label %if.then.L5, label %if.else.L5

if.then.L5:                                       ; preds = %0
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  br label %if.exit.L5

if.else.L5:                                       ; preds = %0
  %3 = load i1, ptr %condition2, align 1
  br i1 %3, label %if.then.L7, label %if.else.L7

if.then.L7:                                       ; preds = %if.else.L5
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  br label %if.exit.L7

if.else.L7:                                       ; preds = %if.else.L5
  %5 = load i1, ptr %condition3, align 1
  br i1 %5, label %if.then.L9, label %if.else.L9

if.then.L9:                                       ; preds = %if.else.L7
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  br label %if.exit.L9

if.else.L9:                                       ; preds = %if.else.L7
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  br label %if.exit.L9

if.exit.L9:                                       ; preds = %if.else.L9, %if.then.L9
  br label %if.exit.L7

if.exit.L7:                                       ; preds = %if.exit.L9, %if.then.L7
  br label %if.exit.L5

if.exit.L5:                                       ; preds = %if.exit.L7, %if.then.L5
  %8 = load i32, ptr %result, align 4
  ret i32 %8
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
