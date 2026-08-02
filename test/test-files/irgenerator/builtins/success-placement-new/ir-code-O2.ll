; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %storage = alloca ptr, align 8
  %1 = tail call ptr @_Z12sAllocUnsafem(i64 8) #2
  %2 = getelementptr inbounds nuw i8, ptr %1, i64 4
  store ptr %1, ptr %storage, align 8
  store i32 7, ptr %1, align 4
  store i32 11, ptr %2, align 4
  %3 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef 7, i32 noundef 11)
  call void @_Z8sDeallocRPh(ptr nonnull %storage) #2
  ret i32 0
}

declare ptr @_Z12sAllocUnsafem(i64) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_Z8sDeallocRPh(ptr) local_unnamed_addr

attributes #0 = { mustprogress noinline norecurse nounwind uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
