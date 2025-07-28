; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Error = type { i32, ptr }

@stderr = external local_unnamed_addr global ptr, align 8
@anon.string.0 = private unnamed_addr constant [117 x i8] c"Program panicked at ./source.spice:2:5: %s\0A2  panic(Error(\22This is an error\22));\0A   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [17 x i8] c"This is an error\00", align 1

; Function Attrs: cold noreturn
define private fastcc void @_Z3foov() unnamed_addr #0 {
  %1 = alloca %struct.Error, align 8
  %2 = load ptr, ptr @stderr, align 8
  call void @_ZN5Error4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(16) %1, ptr nonnull @anon.string.1)
  %3 = getelementptr inbounds nuw i8, ptr %1, i64 8
  %4 = load ptr, ptr %3, align 8
  %5 = call i32 (ptr, ptr, ...) @fprintf(ptr %2, ptr nonnull @anon.string.0, ptr %4) #4
  call void @exit(i32 1)
  unreachable
}

declare void @_ZN5Error4ctorEPKc(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold nofree noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #3 {
  tail call fastcc void @_Z3foov() #5
  unreachable
}

attributes #0 = { cold noreturn }
attributes #1 = { nofree nounwind }
attributes #2 = { cold nofree noreturn nounwind }
attributes #3 = { noinline nounwind optnone uwtable }
attributes #4 = { cold }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
