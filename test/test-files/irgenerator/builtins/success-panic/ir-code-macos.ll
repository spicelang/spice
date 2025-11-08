; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Error = type { i32, ptr }

@__stderrp = external local_unnamed_addr global ptr, align 8
@anon.string.0 = private unnamed_addr constant [117 x i8] c"Program panicked at ./source.spice:2:5: %s\0A2  panic(Error(\22This is an error\22));\0A   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [17 x i8] c"This is an error\00", align 4

define private void @_Z3foov() {
  %1 = alloca %struct.Error, align 8
  %2 = load ptr, ptr @__stderrp, align 8
  call void @_ZN5Error4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(16) %1, ptr @anon.string.1)
  %3 = getelementptr inbounds nuw %struct.Error, ptr %1, i32 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = call i32 (ptr, ptr, ...) @fprintf(ptr %2, ptr @anon.string.0, ptr %4)
  call void @exit(i32 1)
  unreachable
}

declare void @_ZN5Error4ctorEPKc(ptr, ptr)

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z3foov()
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
