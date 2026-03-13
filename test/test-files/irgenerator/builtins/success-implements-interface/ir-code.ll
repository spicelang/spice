; ModuleID = 'source.spice'
source_filename = "source.spice"

@_ZTS5ITest = private constant [7 x i8] c"5ITest\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5ITest = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5ITest }, align 8
@_ZTV5ITest = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5ITest, ptr null] }, align 8
@_ZTS5Test1 = private constant [7 x i8] c"5Test1\00", align 4
@_ZTI5Test1 = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5Test1, ptr @_ZTI5ITest }, align 8
@_ZTV5Test1 = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5Test1, ptr @_ZN5Test17methodAEv] }, align 8
@printf.str.0 = private unnamed_addr constant [18 x i8] c"Method A called!\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [41 x i8] c"Implements interface (ITest, Test1): %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [41 x i8] c"Implements interface (ITest, Test2): %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [40 x i8] c"Implements interface (ITest, bool): %d\0A\00", align 4

define private void @_ZN5Test17methodAEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef 1)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef 0)
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef 0)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
