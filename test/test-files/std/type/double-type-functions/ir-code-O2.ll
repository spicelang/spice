; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'asBool1 == true' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asBool2 == false' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call i1 @_f__void__bool__toBool__double(double 1.000000e+00) #2
  br i1 %1, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %0
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L26:                                  ; preds = %0
  %3 = tail call i1 @_f__void__bool__toBool__double(double 0.000000e+00) #2
  br i1 %3, label %assert.then.L28, label %assert.exit.L28, !prof !1

assert.then.L28:                                  ; preds = %assert.exit.L26
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  tail call void @exit(i32 1) #2
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare i1 @_f__void__bool__toBool__double(double) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
