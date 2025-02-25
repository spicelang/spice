; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [15 x i8] c"Hello syscall!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call i64 @_Z12getRawLengthPKc(ptr nonnull @anon.string.0) #1
  tail call void asm sideeffect "mov x8, $0\0Amov x0, $1\0Amov x1, $2\0Amov x2, $3\0Amov x3, $4\0Amov x4, $5\0Amov x5, $6\0Asvc #0\0A", "r,r,r,r,r,r,r,~{x8},~{x0},~{x1},~{x2},~{x3},~{x4},~{x5},~{dirflag},~{fpsr},~{flags}"(i64 1, i64 1, i64 ptrtoint (ptr @anon.string.0 to i64), i64 %1, i64 0, i64 0, i64 0) #1
  ret i32 0
}

declare i64 @_Z12getRawLengthPKc(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
