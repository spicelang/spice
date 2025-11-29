; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'false' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"false\00", align 4
@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %s\00", align 4

define private noundef zeroext i1 @_Z7condFctv() {
  %result = alloca i1, align 1
  ret i1 false
}

define private noundef ptr @_Z7trueFctv() {
  %result = alloca ptr, align 8
  br i1 false, label %assert.exit.L6, label %assert.then.L6, !prof !5

assert.then.L6:                                   ; preds = %0
  %1 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L6:                                   ; preds = %0
  %2 = load ptr, ptr %result, align 8
  ret ptr %2
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

define private noundef ptr @_Z8falseFctv() {
  %result = alloca ptr, align 8
  ret ptr @anon.string.1
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call noundef zeroext i1 @_Z7condFctv()
  br i1 %1, label %cond.true.L15C26, label %cond.false.L15C26

cond.true.L15C26:                                 ; preds = %0
  %2 = call noundef ptr @_Z7trueFctv()
  br label %cond.exit.L15C26

cond.false.L15C26:                                ; preds = %0
  %3 = call noundef ptr @_Z8falseFctv()
  br label %cond.exit.L15C26

cond.exit.L15C26:                                 ; preds = %cond.false.L15C26, %cond.true.L15C26
  %cond.result = phi ptr [ %2, %cond.true.L15C26 ], [ %3, %cond.false.L15C26 ]
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %cond.result)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
