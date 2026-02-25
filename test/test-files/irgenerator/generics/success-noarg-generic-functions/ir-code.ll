; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'isSame<int, int>()' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [74 x i8] c"Assertion failed: Condition '!isSame<int, string>()' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [80 x i8] c"Assertion failed: Condition 'isSame<double*&, double*&>()' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  br i1 true, label %assert.exit.L9, label %assert.then.L9, !prof !5

assert.then.L9:                                   ; preds = %0
  %1 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %0
  %2 = xor i1 false, true
  br i1 %2, label %assert.exit.L10, label %assert.then.L10, !prof !5

assert.then.L10:                                  ; preds = %assert.exit.L9
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  br i1 true, label %assert.exit.L11, label %assert.then.L11, !prof !5

assert.then.L11:                                  ; preds = %assert.exit.L10
  %4 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L10
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
