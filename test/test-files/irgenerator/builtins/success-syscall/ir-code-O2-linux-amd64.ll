; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [15 x i8] c"Hello syscall!\00", align 4
@SYSCALL_WRITE = external local_unnamed_addr global i16

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %1 = load i16, ptr @SYSCALL_WRITE, align 2
  %2 = zext i16 %1 to i64
  %3 = tail call i64 @_Z12getRawLengthPKc(ptr nonnull @anon.string.0) #1
  tail call void asm sideeffect "movq $0, %rax\0Amovq $1, %rdi\0Amovq $2, %rsi\0Amovq $3, %rdx\0Asyscall\0A", "r,r,r,r,~{rax},~{rdi},~{rsi},~{rdx},~{dirflag},~{fpsr},~{flags}"(i64 %2, i64 1, i64 ptrtoint (ptr @anon.string.0 to i64), i64 %3) #1
  ret i32 0
}

declare i64 @_Z12getRawLengthPKc(ptr) local_unnamed_addr

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
