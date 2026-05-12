use std::fs;
use std::path::Path;

fn visit_dirs(dir: &Path) {
    if let Ok(entries) = fs::read_dir(dir) {
        for entry in entries {
            if let Ok(entry) = entry {
                let path = entry.path();
                eprintln!("- {}", path.display());
                if path.is_dir() {
                    visit_dirs(&path);
                }
            }
        }
    }
}

fn main() {
    if let Ok(current_dir) = std::env::current_dir() {
        eprintln!("{}", current_dir.display());
    }
    visit_dirs(Path::new("."));
}
