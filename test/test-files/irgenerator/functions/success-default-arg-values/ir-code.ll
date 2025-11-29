; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 4
@printf.str.0 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 4

define private noundef ptr @_Z13getTestStringidb(i32 noundef %0, double noundef %1, i1 noundef zeroext %2) {
  %result = alloca ptr, align 8
  %_arg0 = alloca i32, align 4
  %_arg1 = alloca double, align 8
  %_arg2 = alloca i1, align 1
  %_arg3 = alloca double, align 8
  store i32 %0, ptr %_arg0, align 4
  store double %1, ptr %_arg1, align 8
  store i1 %2, ptr %_arg2, align 1
  store double 1.340000e+00, ptr %_arg3, align 8
  ret ptr @anon.string.0
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call noundef ptr @_Z13getTestStringidb(i32 noundef 1, double noundef 3.400000e+00, i1 noundef zeroext true)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
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
